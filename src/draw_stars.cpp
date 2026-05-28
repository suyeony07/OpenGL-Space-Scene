#include "draw_objects.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <cmath>

static GLuint starVAO = 0, starVBO = 0;
static GLuint starProgram = 0;

// 별 위치 (NDC, -1~1)
static float starPositions[200 * 3]; // 별 100개
static float starPhases[100];        // 반짝임 phase 오프셋

static GLuint compileStarShader() {
    const char* vs = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        void main() { gl_Position = vec4(aPos, 1.0); }
    )";
    const char* fs = R"(
        #version 330 core
        uniform float uAlpha;
        uniform vec3 uColor;
        out vec4 fragColor;
        void main() { fragColor = vec4(uColor, uAlpha); }
    )";

    GLuint v = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(v, 1, &vs, nullptr);
    glCompileShader(v);

    GLuint f = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f, 1, &fs, nullptr);
    glCompileShader(f);

    GLuint prog = glCreateProgram();
    glAttachShader(prog, v);
    glAttachShader(prog, f);
    glLinkProgram(prog);

    glDeleteShader(v);
    glDeleteShader(f);
    return prog;
}

void initStars() {
    srand(42);
    for (int i = 0; i < 100; i++) {
        starPositions[i * 3 + 0] = ((float)rand() / RAND_MAX) * 2.0f - 1.0f; // x
        starPositions[i * 3 + 1] = ((float)rand() / RAND_MAX) * 2.0f - 1.0f; // y
        starPositions[i * 3 + 2] = 0.0f;
        starPhases[i] = ((float)rand() / RAND_MAX) * 6.28f; // 0~2π
    }

    glGenVertexArrays(1, &starVAO);
    glGenBuffers(1, &starVBO);
    glBindVertexArray(starVAO);
    glBindBuffer(GL_ARRAY_BUFFER, starVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(starPositions), starPositions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    starProgram = compileStarShader();
}

void drawStars() {
    if (starVAO == 0) initStars();

    float t = (float)glfwGetTime();

    glUseProgram(starProgram);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPointSize(3.0f);

    GLint alphaLoc = glGetUniformLocation(starProgram, "uAlpha");
    GLint colorLoc = glGetUniformLocation(starProgram, "uColor");

    glBindVertexArray(starVAO);

    for (int i = 0; i < 100; i++) {
        float alpha = 0.4f + 0.6f * (0.5f + 0.5f * sinf(t * 2.0f + starPhases[i]));
        glUniform1f(alphaLoc, alpha);
        glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);
        glDrawArrays(GL_POINTS, i, 1);
    }

    glDisable(GL_BLEND);
}