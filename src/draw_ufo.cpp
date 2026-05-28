// src/draw_ufo.cpp
#include "common.h"
#include "draw_objects.h"
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

static std::vector<float> ellF(float cx, float cy, float rx, float ry, int seg) {
    std::vector<float> v;
    const float PI = 3.14159265f;
    v.push_back(cx); v.push_back(cy); v.push_back(0.f);
    for (int i = 0; i <= seg; i++) {
        float a = 2.f * PI * i / seg;
        v.push_back(cx + rx * cosf(a));
        v.push_back(cy + ry * sinf(a));
        v.push_back(0.f);
    }
    return v;
}

static std::vector<float> ellL(float cx, float cy, float rx, float ry, int seg) {
    std::vector<float> v;
    const float PI = 3.14159265f;
    for (int i = 0; i < seg; i++) {
        float a = 2.f * PI * i / seg;
        v.push_back(cx + rx * cosf(a));
        v.push_back(cy + ry * sinf(a));
        v.push_back(0.f);
    }
    return v;
}

static void draw(const std::vector<float>& v, GLenum mode) {
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao); glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, v.size()*sizeof(float), v.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glDrawArrays(mode, 0, (GLsizei)(v.size()/3));
    glDeleteBuffers(1, &vbo); glDeleteVertexArrays(1, &vao);
}

static void drawLine(float x1, float y1, float x2, float y2) {
    draw({ x1,y1,0, x2,y2,0 }, GL_LINES);
}

void drawUFO() {
    initShader();
    glUseProgram(s_prog);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const float PI = 3.14159265f;
    const float cx = 0.70f;
    const float cy = 0.58f;

    // ── 빔 (청백색 그라데이션) ─────────────────────────
    float bT = cy - 0.07f;
    float bB = cy - 0.42f;
    float bTw = 0.05f;
    float bBw = 0.13f;

    int steps = 10;
    for (int i = 0; i < steps; i++) {
        float t0 = (float)i     / steps;
        float t1 = (float)(i+1) / steps;
        float y0 = bT - t0 * (bT - bB);
        float y1 = bT - t1 * (bT - bB);
        float hw0 = bTw + t0 * (bBw - bTw);
        float hw1 = bTw + t1 * (bBw - bTw);
        float alpha = 0.40f * (1.0f - t0);
        col(0.55f, 0.85f, 1.00f, alpha);  // 청백색
        draw({
            cx-hw0, y0, 0,  cx+hw0, y0, 0,  cx+hw1, y1, 0,
            cx-hw0, y0, 0,  cx+hw1, y1, 0,  cx-hw1, y1, 0,
        }, GL_TRIANGLES);
    }


    // ── 접시 (은청색 금속) ────────────────────────────
    col(0.55f, 0.65f, 0.78f);  // 본체
    draw(ellF(cx, cy, 0.20f, 0.06f, 60), GL_TRIANGLE_FAN);
    col(0.72f, 0.82f, 0.92f);  // 하이라이트
    draw(ellF(cx, cy+0.015f, 0.17f, 0.038f, 60), GL_TRIANGLE_FAN);
    col(0.20f, 0.28f, 0.40f);  // 외곽선
    draw(ellL(cx, cy, 0.20f, 0.06f, 60), GL_LINE_LOOP);

    // 노즐 4개
    float nX[4] = {cx-0.11f, cx-0.045f, cx+0.045f, cx+0.11f};
    for (int k = 0; k < 4; k++) {
        col(0.18f, 0.22f, 0.32f);
        draw(ellF(nX[k], cy-0.055f, 0.013f, 0.008f, 12), GL_TRIANGLE_FAN);
        col(0.10f, 0.15f, 0.25f);
        draw(ellL(nX[k], cy-0.055f, 0.013f, 0.008f, 12), GL_LINE_LOOP);
    }

    // 양옆 불빛 (차가운 청백)
    col(0.70f, 0.90f, 1.00f);
    draw(ellF(cx-0.220f, cy, 0.025f, 0.020f, 20), GL_TRIANGLE_FAN);
    draw(ellF(cx+0.220f, cy, 0.025f, 0.020f, 20), GL_TRIANGLE_FAN);
    col(0.20f, 0.28f, 0.40f);
    draw(ellL(cx-0.220f, cy, 0.025f, 0.020f, 20), GL_LINE_LOOP);
    draw(ellL(cx+0.220f, cy, 0.025f, 0.020f, 20), GL_LINE_LOOP);

    // ── 돔 (짙은 강화유리) ───────────────────────────
    float dcy  = cy + 0.115f;
    float domR = 0.115f;

    col(0.10f, 0.20f, 0.38f, 0.88f);  // 짙은 네이비 반투명
    draw(ellF(cx, dcy, domR, domR, 40), GL_TRIANGLE_FAN);
    col(0.40f, 0.60f, 0.85f, 0.40f);  // 하이라이트 반사
    draw(ellF(cx-0.025f, dcy+0.02f, domR*0.5f, domR*0.35f, 30), GL_TRIANGLE_FAN);
    col(0.20f, 0.35f, 0.58f);
    draw(ellL(cx, dcy, domR, domR, 40), GL_LINE_LOOP);

    // 돔-접시 연결
    col(0.55f, 0.65f, 0.78f);
    draw(ellF(cx, cy+0.060f, domR, 0.018f, 40), GL_TRIANGLE_FAN);
    col(0.20f, 0.28f, 0.40f);
    draw(ellL(cx, cy, 0.20f, 0.06f, 60), GL_LINE_LOOP);

    // ── 외계인 (청록 피부) ────────────────────────────
    float hcy = dcy + 0.005f;
    float acy = hcy - 0.070f;

    // 몸통
    col(0.25f, 0.55f, 0.55f);
    draw(ellF(cx, acy, 0.030f, 0.020f, 20), GL_TRIANGLE_FAN);
    col(0.10f, 0.30f, 0.35f);
    draw(ellL(cx, acy, 0.030f, 0.020f, 20), GL_LINE_LOOP);

    // 팔
    col(0.25f, 0.55f, 0.55f);
    drawLine(cx-0.030f, acy, cx-0.075f, acy+0.040f);
    drawLine(cx-0.075f, acy+0.040f, cx-0.055f, acy+0.068f);
    drawLine(cx-0.075f, acy+0.040f, cx-0.040f, acy+0.068f);
    drawLine(cx+0.030f, acy, cx+0.075f, acy+0.040f);
    drawLine(cx+0.075f, acy+0.040f, cx+0.055f, acy+0.068f);
    drawLine(cx+0.075f, acy+0.040f, cx+0.040f, acy+0.068f);

    // 하트 (차가운 분홍→보라)
    col(0.75f, 0.20f, 0.65f);
    draw(ellF(cx-0.013f, acy+0.075f, 0.013f, 0.013f, 14), GL_TRIANGLE_FAN);
    draw(ellF(cx+0.013f, acy+0.075f, 0.013f, 0.013f, 14), GL_TRIANGLE_FAN);
    draw({
        cx-0.026f, acy+0.075f, 0,
        cx+0.026f, acy+0.075f, 0,
        cx,        acy+0.050f, 0,
    }, GL_TRIANGLES);

    // 머리
    col(0.25f, 0.55f, 0.55f);
    draw(ellF(cx, hcy, 0.045f, 0.052f, 30), GL_TRIANGLE_FAN);
    col(0.10f, 0.30f, 0.35f);
    draw(ellL(cx, hcy, 0.045f, 0.052f, 30), GL_LINE_LOOP);

    // 눈 흰자
    col(0.88f, 0.95f, 1.00f);
    draw(ellF(cx-0.016f, hcy+0.006f, 0.013f, 0.016f, 16), GL_TRIANGLE_FAN);
    draw(ellF(cx+0.016f, hcy+0.006f, 0.013f, 0.016f, 16), GL_TRIANGLE_FAN);
    // 눈동자 (짙은 파랑)
    col(0.02f, 0.05f, 0.20f);
    draw(ellF(cx-0.016f, hcy+0.006f, 0.008f, 0.010f, 14), GL_TRIANGLE_FAN);
    draw(ellF(cx+0.016f, hcy+0.006f, 0.008f, 0.010f, 14), GL_TRIANGLE_FAN);
    // 반짝임
    col(1.0f, 1.0f, 1.0f);
    draw(ellF(cx-0.019f, hcy+0.010f, 0.003f, 0.003f, 8), GL_TRIANGLE_FAN);
    draw(ellF(cx+0.013f, hcy+0.010f, 0.003f, 0.003f, 8), GL_TRIANGLE_FAN);

    // 입
    col(0.10f, 0.30f, 0.35f);
    {
        std::vector<float> mouth;
        for (int i = 0; i <= 10; i++) {
            float a = PI*1.15f + PI*0.70f*i/10.f;
            mouth.push_back(cx + 0.026f*cosf(a));
            mouth.push_back(hcy - 0.020f + 0.011f*sinf(a));
            mouth.push_back(0.f);
        }
        draw(mouth, GL_LINE_STRIP);
    }

    // 안테나
    col(0.10f, 0.30f, 0.35f);
    for (float d = -0.003f; d <= 0.003f; d += 0.0015f) {
        drawLine(cx-0.013f+d, hcy+0.050f, cx-0.028f+d, hcy+0.078f);
        drawLine(cx+0.013f+d, hcy+0.050f, cx+0.028f+d, hcy+0.078f);
    }

    // 안테나 하트 (보라)
    col(0.75f, 0.20f, 0.65f);
    draw(ellF(cx-0.034f, hcy+0.085f, 0.007f, 0.007f, 12), GL_TRIANGLE_FAN);
    draw(ellF(cx-0.022f, hcy+0.085f, 0.007f, 0.007f, 12), GL_TRIANGLE_FAN);
    draw({ cx-0.041f, hcy+0.085f, 0, cx-0.015f, hcy+0.085f, 0, cx-0.028f, hcy+0.072f, 0 }, GL_TRIANGLES);

    col(0.75f, 0.20f, 0.65f);
    draw(ellF(cx+0.022f, hcy+0.085f, 0.007f, 0.007f, 12), GL_TRIANGLE_FAN);
    draw(ellF(cx+0.034f, hcy+0.085f, 0.007f, 0.007f, 12), GL_TRIANGLE_FAN);
    draw({ cx+0.015f, hcy+0.085f, 0, cx+0.041f, hcy+0.085f, 0, cx+0.028f, hcy+0.072f, 0 }, GL_TRIANGLES);

    glDisable(GL_BLEND);
}