// src/draw_moon.cpp
#include "draw_objects.h"
#include <glad/glad.h>
#include <cmath>
#include <vector>

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

static void drawFill(float cx, float cy, float rx, float ry, int seg) {
    std::vector<float> v;
    v.push_back(cx); v.push_back(cy); v.push_back(0.f);
    for (int i = 0; i <= seg; i++) {
        float a = 2.f * 3.14159265f * i / seg;
        v.push_back(cx + rx * cosf(a));
        v.push_back(cy + ry * sinf(a));
        v.push_back(0.f);
    }
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao); glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, v.size()*sizeof(float), v.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, (GLsizei)(v.size()/3));
    glDeleteBuffers(1, &vbo); glDeleteVertexArrays(1, &vao);
}

static void drawLine(float cx, float cy, float rx, float ry, int seg) {
    std::vector<float> v;
    for (int i = 0; i < seg; i++) {
        float a = 2.f * 3.14159265f * i / seg;
        v.push_back(cx + rx * cosf(a));
        v.push_back(cy + ry * sinf(a));
        v.push_back(0.f);
    }
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao); glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, v.size()*sizeof(float), v.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_LINE_LOOP, 0, (GLsizei)(v.size()/3));
    glDeleteBuffers(1, &vbo); glDeleteVertexArrays(1, &vao);
}

static void drawTriangle(float x0, float y0, float x1, float y1, float x2, float y2) {
    float v[] = { x0,y0,0, x1,y1,0, x2,y2,0 };
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao); glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDeleteBuffers(1, &vbo); glDeleteVertexArrays(1, &vao);
}
static void drawRect(float x0, float y0, float x1, float y1) {
    float v[] = { x0,y0,0, x1,y0,0, x1,y1,0, x0,y0,0, x1,y1,0, x0,y1,0 };
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao); glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDeleteBuffers(1, &vbo); glDeleteVertexArrays(1, &vao);
}
void drawMoon(float x, float y) {
    initShader();
    glUseProgram(s_prog);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float r = 0.14f;

    // 글로우 (은은한 빛)
    col(0.95f, 0.90f, 0.55f, 0.08f);
    drawFill(x, y, r*1.6f, r*1.6f, 48);
    col(0.95f, 0.88f, 0.50f, 0.12f);
    drawFill(x, y, r*1.3f, r*1.3f, 48);

    // 그림자
    col(0.60f, 0.50f, 0.25f, 0.30f);
    drawFill(x + r*0.05f, y - r*0.05f, r*1.03f, r*1.03f, 48);

    // 본체 (밝은 노란색)
    col(0.97f, 0.92f, 0.62f);
    drawFill(x, y, r, r, 48);

    // 밝은 면 (왼쪽 위 하이라이트)
    col(1.00f, 0.97f, 0.80f, 0.60f);
    drawFill(x - r*0.25f, y + r*0.30f, r*0.50f, r*0.45f, 30);

    // 두 번째 하이라이트 (작고 강하게)
    col(1.00f, 1.00f, 0.92f, 0.40f);
    drawFill(x - r*0.35f, y + r*0.40f, r*0.25f, r*0.20f, 20);

    // 어두운 면 (오른쪽 아래)
    col(0.82f, 0.72f, 0.42f, 0.30f);
    drawFill(x + r*0.28f, y - r*0.22f, r*0.48f, r*0.42f, 28);

    // 크레이터 1 (큰)
    col(0.82f, 0.75f, 0.48f);
    drawFill(x + r*0.32f, y + r*0.20f, r*0.18f, r*0.18f, 18);
    col(0.75f, 0.68f, 0.42f, 0.60f);
    drawFill(x + r*0.34f, y + r*0.22f, r*0.12f, r*0.12f, 14);
    col(0.88f, 0.82f, 0.55f, 0.35f);
    drawFill(x + r*0.28f, y + r*0.26f, r*0.06f, r*0.06f, 10);

    // 크레이터 2
    col(0.80f, 0.72f, 0.44f);
    drawFill(x - r*0.28f, y - r*0.30f, r*0.15f, r*0.15f, 16);
    col(0.73f, 0.66f, 0.40f, 0.55f);
    drawFill(x - r*0.30f, y - r*0.28f, r*0.09f, r*0.09f, 12);

    // 크레이터 3
    col(0.78f, 0.70f, 0.42f);
    drawFill(x + r*0.08f, y - r*0.42f, r*0.12f, r*0.12f, 14);

    // 크레이터 4 (위쪽 작은)
    col(0.80f, 0.73f, 0.45f);
    drawFill(x - r*0.12f, y + r*0.48f, r*0.08f, r*0.08f, 12);

    // 크레이터 5
    col(0.79f, 0.71f, 0.43f);
    drawFill(x - r*0.48f, y + r*0.08f, r*0.07f, r*0.07f, 10);

    // 크레이터 6 (아주 작은)
    col(0.81f, 0.74f, 0.46f);
    drawFill(x + r*0.45f, y - r*0.10f, r*0.05f, r*0.05f, 8);

    // 표면 질감 (미세한 점들)
    col(0.85f, 0.78f, 0.50f, 0.25f);
    drawFill(x + r*0.15f, y + r*0.40f, r*0.04f, r*0.04f, 8);
    drawFill(x - r*0.40f, y - r*0.15f, r*0.03f, r*0.03f, 8);
    drawFill(x + r*0.38f, y + r*0.38f, r*0.03f, r*0.03f, 8);

    // 외곽선 (따뜻한 갈색, 약간 두껍게)
    col(0.70f, 0.60f, 0.32f, 0.80f);
    drawLine(x, y, r, r, 48);

    // ── 깃대 ─────────────────────────────────────
    float poleX = x;
    float poleBot = y + r * 0.6f;
    float poleTop = y + r * 1.8f;

    col(0.50f, 0.50f, 0.50f);
    drawRect(poleX - 0.003f, poleBot, poleX + 0.005f, poleTop);
    col(0.92f, 0.92f, 0.92f);
    drawRect(poleX - 0.002f, poleBot, poleX + 0.003f, poleTop);

    // 깃대 꼭대기 동그라미
    col(0.85f, 0.85f, 0.85f);
    drawFill(poleX, poleTop + 0.005f, 0.005f, 0.005f, 10);

    // ── 깃발 ─────────────────────────────────────
    float flagL = poleX + 0.003f;
    float flagR = poleX + 0.08f;
    float flagTop = poleTop;
    float flagBot = poleTop - 0.045f;

    // 깃발 그림자
    col(0.65f, 0.08f, 0.08f, 0.30f);
    drawRect(flagL + 0.002f, flagBot - 0.002f, flagR + 0.002f, flagTop - 0.002f);

    // 깃발 본체
    col(0.95f, 0.15f, 0.12f);
    drawRect(flagL, flagBot, flagR, flagTop);

    // 깃발 하이라이트 (위쪽 밝게)
    col(1.0f, 0.35f, 0.30f, 0.40f);
    drawRect(flagL, flagTop - 0.012f, flagR, flagTop);

    // 깃발 어두운 부분 (아래)
    col(0.72f, 0.08f, 0.06f, 0.45f);
    drawRect(flagL, flagBot, flagR, flagBot + 0.012f);

    // 별 (다윗의 별 모양)
    col(1.0f, 1.0f, 1.0f);
    float starCx = flagL + 0.04f;
    float starCy = flagTop - 0.022f;
    float ss = 0.010f;
    drawTriangle(starCx, starCy + ss, starCx - ss, starCy - ss*0.4f, starCx + ss, starCy - ss*0.4f);
    drawTriangle(starCx, starCy - ss*0.8f, starCx - ss, starCy + ss*0.2f, starCx + ss, starCy + ss*0.2f);

    // 깃대 바닥 고정 장치
    col(0.65f, 0.60f, 0.45f);
    drawFill(poleX, poleBot, 0.014f, 0.009f, 12);
    col(0.55f, 0.50f, 0.38f);
    drawLine(poleX, poleBot, 0.014f, 0.009f, 12);

    glDisable(GL_BLEND);
}