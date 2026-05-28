// src/draw_ring_planet.cpp
#define _USE_MATH_DEFINES
#include <cmath>
#include "draw_objects.h"
#include "common.h"
#include <vector>
#include <chrono>

// ── shader ────────────────────────────────────────────────
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

// ── 기존 draw 함수들 (그대로 유지) ───────────────────────
static void drawLineLoop(const std::vector<float>& verts) {
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao); glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verts.size()*sizeof(float), verts.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_LINE_LOOP, 0, (GLsizei)(verts.size()/3));
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo); glDeleteVertexArrays(1, &vao);
}

static void drawTriangleFan(const std::vector<float>& verts) {
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao); glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verts.size()*sizeof(float), verts.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, (GLsizei)(verts.size()/3));
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo); glDeleteVertexArrays(1, &vao);
}

static void drawLineStrip(const std::vector<float>& verts) {
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao); glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verts.size()*sizeof(float), verts.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)(verts.size()/3));
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo); glDeleteVertexArrays(1, &vao);
}

static std::vector<float> makeEllipse(
    float cx, float cy, float rx, float ry, int segments, float rotAngle = 0.0f)
{
    std::vector<float> verts;
    for (int i = 0; i <= segments; i++) {
        float a  = 2.0f * (float)M_PI * i / segments;
        float lx = rx * cosf(a);
        float ly = ry * sinf(a);
        float rx2 = lx * cosf(rotAngle) - ly * sinf(rotAngle);
        float ry2 = lx * sinf(rotAngle) + ly * cosf(rotAngle);
        verts.push_back(cx + rx2);
        verts.push_back(cy + ry2);
        verts.push_back(0.0f);
    }
    return verts;
}

// 채운 원 (TRIANGLE_FAN용 정점, 중심점 포함)
static std::vector<float> makeEllipseFill(
    float cx, float cy, float rx, float ry, int segments)
{
    std::vector<float> verts;
    verts.push_back(cx); verts.push_back(cy); verts.push_back(0.f);
    for (int i = 0; i <= segments; i++) {
        float a = 2.f * (float)M_PI * i / segments;
        verts.push_back(cx + rx * cosf(a));
        verts.push_back(cy + ry * sinf(a));
        verts.push_back(0.f);
    }
    return verts;
}

void drawRingPlanet() {
    initShader();
    glUseProgram(s_prog);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const float cx  = -0.70f;
    const float cy  =  0.625f;
    const int   seg =  64;
    const float pR  = 0.10f;
    const float ringOutRx = 0.20f, ringOutRy = 0.045f;
    const float ringInRx  = 0.14f, ringInRy  = 0.030f;

    static auto startTime = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    float elapsed = std::chrono::duration<float>(now - startTime).count();
    float rot = elapsed * 0.5f;

    // ── 고리 뒷부분 (위쪽 반) ────────────────────────────
    col(0.72f, 0.58f, 0.38f, 0.50f);
    {
        std::vector<float> back;
        for (int i = 0; i <= seg; i++) {
            float a = (float)M_PI * i / seg;  // 0 ~ π (위쪽)
            back.push_back(cx + ringOutRx * cosf(a));
            back.push_back(cy + ringOutRy * sinf(a));
            back.push_back(0.f);
        }
        drawLineStrip(back);
    }
    col(0.55f, 0.42f, 0.22f, 0.55f);
    drawLineLoop(makeEllipse(cx, cy, ringOutRx, ringOutRy, seg));
    drawLineLoop(makeEllipse(cx, cy, ringInRx,  ringInRy,  seg));

    // ── 행성 본체 채우기 ──────────────────────────────────
    col(0.18f, 0.32f, 0.70f);  // 파란 가스 행성
    drawTriangleFan(makeEllipseFill(cx, cy, pR, pR, seg));

    // 대기 띠 1
    col(0.50f, 0.68f, 0.92f, 0.50f);
    drawTriangleFan(makeEllipseFill(cx, cy + pR*0.15f, pR*0.92f, pR*0.22f, seg));

    // 대기 띠 2 (반대 방향 착시용)
    col(0.28f, 0.48f, 0.82f, 0.38f);
    drawTriangleFan(makeEllipseFill(cx, cy - pR*0.18f, pR*0.88f, pR*0.16f, seg));

    // 본체 외곽선
    col(0.12f, 0.22f, 0.50f);
    drawLineLoop(makeEllipse(cx, cy, pR, pR, seg));

    // ── 자전선 (기존 유지, 색만 변경) ────────────────────
    col(0.65f, 0.82f, 1.00f, 0.60f);
    drawLineLoop(makeEllipse(cx, cy, pR*0.95f, pR*0.25f, seg, rot));
    col(0.55f, 0.72f, 0.92f, 0.45f);
    drawLineLoop(makeEllipse(cx, cy, pR*0.95f, pR*0.25f, seg, rot + (float)M_PI*0.5f));

    // ── 고리 앞부분 (아래쪽 반) ──────────────────────────
    col(0.85f, 0.70f, 0.42f, 0.65f);
    {
        std::vector<float> front;
        for (int i = 0; i <= seg; i++) {
            float a = (float)M_PI * i / seg;
            front.push_back(cx + ringOutRx * cosf(a));
            front.push_back(cy - ringOutRy * sinf(a));
            front.push_back(0.f);
        }
        drawLineStrip(front);
    }
    {
        std::vector<float> frontIn;
        for (int i = 0; i <= seg; i++) {
            float a = (float)M_PI * i / seg;
            frontIn.push_back(cx + ringInRx * cosf(a));
            frontIn.push_back(cy - ringInRy * sinf(a));
            frontIn.push_back(0.f);
        }
        drawLineStrip(frontIn);
    }

    // ── 고리 마커 (기존 유지, 색만 변경) ─────────────────
    col(0.90f, 0.75f, 0.50f, 0.80f);
    {
        float d = 0.012f;
        for (int i = 0; i < 4; i++) {
            float angle = rot + (float)M_PI * 0.5f * i;
            float mx = cx + ringOutRx * cosf(angle);
            float my = cy + ringOutRy * sinf(angle);
            drawLineStrip({ mx-d, my, 0.f, mx+d, my, 0.f });
            drawLineStrip({ mx, my-d, 0.f, mx, my+d, 0.f });
        }
    }
    col(0.70f, 0.58f, 0.35f, 0.70f);
    {
        float d = 0.010f;
        for (int i = 0; i < 3; i++) {
            float angle = -rot + (float)M_PI * 2.f / 3.f * i;
            float mx = cx + ringInRx * cosf(angle);
            float my = cy + ringInRy * sinf(angle);
            drawLineStrip({ mx-d, my, 0.f, mx+d, my, 0.f });
            drawLineStrip({ mx, my-d, 0.f, mx, my+d, 0.f });
        }
    }

    glDisable(GL_BLEND);
}