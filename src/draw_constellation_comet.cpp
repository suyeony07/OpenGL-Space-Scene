// src/draw_constellation_comet.cpp
#include "draw_objects.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <vector>

// ── 자체 shader (색상 uniform) ─────────────────────────────
static GLuint s_prog = 0;
static GLint  s_colorLoc = -1;

static void initShader() {
    if (s_prog != 0) return;
    const char* vs = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        void main() { gl_Position = vec4(aPos, 1.0); }
    )";
    const char* fs = R"(
        #version 330 core
        out vec4 fragColor;
        uniform vec4 uColor;
        void main() { fragColor = uColor; }
    )";
    GLuint v = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(v, 1, &vs, nullptr); glCompileShader(v);
    GLuint f = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f, 1, &fs, nullptr); glCompileShader(f);
    s_prog = glCreateProgram();
    glAttachShader(s_prog, v); glAttachShader(s_prog, f);
    glLinkProgram(s_prog);
    glDeleteShader(v); glDeleteShader(f);
    s_colorLoc = glGetUniformLocation(s_prog, "uColor");
}

static void col(float r, float g, float b, float a = 1.f) {
    glUniform4f(s_colorLoc, r, g, b, a);
}

static void drawPrimitive(const float* verts, int count, GLenum mode) {
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glDrawArrays(mode, 0, count / 3);
    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void drawConstellationAndComet() {
    initShader();
    glUseProgram(s_prog);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // ── 1. 별자리 연결선 (차분한 청회색) ──────────────────
    static const float constellationLines[] = {
        -0.72f,  0.26f, 0.0f,   -0.48f,  0.10f, 0.0f,
        -0.20f,  0.24f, 0.0f,   -0.22f,  0.10f, 0.0f,
        -0.22f,  0.10f, 0.0f,   -0.48f,  0.10f, 0.0f,
        -0.48f,  0.10f, 0.0f,   -0.48f, -0.08f, 0.0f,
        -0.48f, -0.08f, 0.0f,   -0.65f, -0.20f, 0.0f,
        -0.48f, -0.08f, 0.0f,   -0.35f, -0.20f, 0.0f,
        -0.65f, -0.20f, 0.0f,   -0.65f, -0.38f, 0.0f,
        -0.82f,  0.10f, 0.0f,   -0.72f,  0.26f, 0.0f,
    };
    col(0.45f, 0.60f, 0.85f, 0.75f);  // 청회색 반투명
    glLineWidth(1.2f);
    drawPrimitive(constellationLines, sizeof(constellationLines)/sizeof(float), GL_LINES);

    // 일반 별
    static const float starPos[] = {
        -0.72f,  0.26f, 0.0f,
        -0.20f,  0.24f, 0.0f,
        -0.48f,  0.10f, 0.0f,
        -0.22f,  0.10f, 0.0f,
        -0.48f, -0.08f, 0.0f,
        -0.65f, -0.20f, 0.0f,
        -0.35f, -0.20f, 0.0f,
        -0.65f, -0.38f, 0.0f,
        -0.82f,  0.10f, 0.0f,
    };
    col(0.75f, 0.88f, 1.00f);  // 밝은 청백
    glPointSize(4.5f);
    drawPrimitive(starPos, sizeof(starPos)/sizeof(float), GL_POINTS);

    // Spica 강조
    static const float spica[] = { -0.48f, -0.08f, 0.0f };
    col(1.00f, 1.00f, 0.85f);  // 따뜻한 흰색 (밝은 별)
    glPointSize(9.0f);
    drawPrimitive(spica, sizeof(spica)/sizeof(float), GL_POINTS);

    // ── 2. 배경 별 (흰색~청백) ────────────────────────────
    static const float bgStars[] = {
         0.15f,  0.28f, 0.0f,   0.48f,  0.20f, 0.0f,
         0.75f,  0.28f, 0.0f,   0.88f,  0.12f, 0.0f,
        -0.88f, -0.02f, 0.0f,   0.05f,  0.05f, 0.0f,
         0.35f, -0.05f, 0.0f,   0.72f, -0.10f, 0.0f,
         0.88f, -0.18f, 0.0f,  -0.85f, -0.28f, 0.0f,
        -0.42f, -0.32f, 0.0f,   0.10f, -0.25f, 0.0f,
         0.45f, -0.35f, 0.0f,   0.82f, -0.30f, 0.0f,
        -0.78f, -0.52f, 0.0f,  -0.28f, -0.48f, 0.0f,
         0.18f, -0.55f, 0.0f,   0.55f, -0.50f, 0.0f,
         0.85f, -0.58f, 0.0f,  -0.62f, -0.68f, 0.0f,
        -0.18f, -0.75f, 0.0f,   0.22f, -0.72f, 0.0f,
         0.65f, -0.78f, 0.0f,   0.88f, -0.82f, 0.0f,
    };
    col(0.85f, 0.90f, 1.00f, 0.80f);
    glPointSize(2.5f);
    drawPrimitive(bgStars, sizeof(bgStars)/sizeof(float), GL_POINTS);

    // ── 3. 혜성 핵 ────────────────────────────────────────
    // 광환
    static const float halo[] = {
        0.49f, -0.16f, 0.0f,   0.41f, -0.16f, 0.0f,
        0.49f, -0.24f, 0.0f,   0.41f, -0.24f, 0.0f,
        0.45f, -0.14f, 0.0f,   0.45f, -0.26f, 0.0f,
        0.51f, -0.20f, 0.0f,   0.39f, -0.20f, 0.0f,
    };
    col(0.70f, 0.90f, 1.00f, 0.50f);  // 청백 반투명
    glPointSize(3.5f);
    drawPrimitive(halo, sizeof(halo)/sizeof(float), GL_POINTS);

    // 핵
    static const float cometHead[] = { 0.45f, -0.20f, 0.0f };
    col(0.90f, 0.97f, 1.00f);  // 거의 흰색
    glPointSize(11.0f);
    drawPrimitive(cometHead, sizeof(cometHead)/sizeof(float), GL_POINTS);
    col(1.00f, 1.00f, 1.00f);
    glPointSize(5.0f);
    drawPrimitive(cometHead, sizeof(cometHead)/sizeof(float), GL_POINTS);

    // 꼬리 (청백 → 투명 느낌을 여러 단계로)
    static const float tailVerts[] = {
        0.45f, -0.20f, 0.0f,   0.89f, -0.20f, 0.0f,
        0.45f, -0.20f, 0.0f,   0.88f, -0.11f, 0.0f,
        0.45f, -0.20f, 0.0f,   0.85f, -0.03f, 0.0f,
        0.45f, -0.20f, 0.0f,   0.78f,  0.03f, 0.0f,
        0.45f, -0.20f, 0.0f,   0.88f, -0.29f, 0.0f,
        0.45f, -0.20f, 0.0f,   0.85f, -0.37f, 0.0f,
        0.45f, -0.20f, 0.0f,   0.78f, -0.43f, 0.0f,
    };
    col(0.65f, 0.85f, 1.00f, 0.55f);  // 청백 반투명
    glLineWidth(1.5f);
    drawPrimitive(tailVerts, sizeof(tailVerts)/sizeof(float), GL_LINES);

    // 궤적 (지나온 흔적, 점점 흐리게)
    static const float trailDots[] = {
        0.33f, -0.06f, 0.0f,   0.22f, -0.16f, 0.0f,
        0.12f, -0.26f, 0.0f,   0.02f, -0.35f, 0.0f,
       -0.07f, -0.43f, 0.0f,
    };
    col(0.55f, 0.75f, 1.00f, 0.45f);
    glPointSize(2.8f);
    drawPrimitive(trailDots, sizeof(trailDots)/sizeof(float), GL_POINTS);

    // ── 4. 유성우 (움직이는) ──────────────────────────────
    float t = (float)glfwGetTime();

    // 유성 6개, 각각 다른 주기/위치로 날아감
    struct Meteor {
        float startX, startY;  // 시작 위치
        float dx, dy;          // 방향 (정규화)
        float speed;           // 속도
        float length;          // 꼬리 길이
        float period;          // 주기 (초)
        float offset;          // 시간 오프셋
    };

    static const Meteor meteors[] = {
        { -0.50f,  0.10f, -0.70f, -0.70f, 0.90f, 0.18f, 2.5f, 0.0f },
        { -0.10f,  0.20f, -0.68f, -0.72f, 0.75f, 0.14f, 3.2f, 0.8f },
        {  0.30f,  0.15f, -0.72f, -0.68f, 1.00f, 0.20f, 2.8f, 1.6f },
        { -0.30f,  0.25f, -0.65f, -0.75f, 0.85f, 0.16f, 3.8f, 2.3f },
        {  0.10f,  0.30f, -0.70f, -0.70f, 0.70f, 0.12f, 2.2f, 3.5f },
        { -0.70f,  0.20f, -0.68f, -0.72f, 0.95f, 0.17f, 4.0f, 1.2f },
    };

    for (const auto& m : meteors) {
        // 0~1 진행도 (주기마다 리셋)
        float progress = fmodf((t + m.offset) * m.speed / 1.5f, m.period) / m.period;
        if (progress > 0.85f) continue;  // 끝부분엔 안 보이게 (깜빡임 방지)

        float headX = m.startX + m.dx * progress * 1.5f;
        float headY = m.startY + m.dy * progress * 1.5f;
        float tailX = headX - m.dx * m.length;
        float tailY = headY - m.dy * m.length;

        // 꼬리 3단계 (굵기/투명도 다르게)
        glLineWidth(2.5f);
        col(1.00f, 1.00f, 1.00f, 0.90f);
        { float v[] = { headX, headY, 0, headX - m.dx*m.length*0.3f, headY - m.dy*m.length*0.3f, 0 };
          drawPrimitive(v, 6, GL_LINES); }

        glLineWidth(1.8f);
        col(0.80f, 0.92f, 1.00f, 0.55f);
        { float v[] = { headX - m.dx*m.length*0.3f, headY - m.dy*m.length*0.3f, 0, tailX, tailY, 0 };
          drawPrimitive(v, 6, GL_LINES); }

        // 머리 점
        glPointSize(4.5f);
        col(1.00f, 1.00f, 1.00f, 1.00f);
        { float v[] = { headX, headY, 0 };
          drawPrimitive(v, 3, GL_POINTS); }
    }
}