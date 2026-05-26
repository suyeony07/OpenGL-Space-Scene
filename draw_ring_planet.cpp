// src/draw_ring_planet.cpp
// 담당 브랜치: feature/ring-planet
// 담당자: 윤지우 (20243150)
//
// 좌표 범위: x[-0.95 ~ -0.45], y[0.35 ~ 0.90]
// 중심: (-0.70, 0.625)

#define _USE_MATH_DEFINES
#include <cmath>
#include "draw_objects.h"
#include "common.h"
#include <vector>
#include <chrono>

// GL_LINE_LOOP 으로 그리기
static void drawLineLoop(const std::vector<float>& verts) {
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 verts.size() * sizeof(float),
                 verts.data(),
                 GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_LINE_LOOP, 0, static_cast<GLsizei>(verts.size() / 3));

    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

// GL_LINE_STRIP 으로 그리기
static void drawLineStrip(const std::vector<float>& verts) {
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 verts.size() * sizeof(float),
                 verts.data(),
                 GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(verts.size() / 3));

    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

// 원/타원 정점 생성 (rotAngle: 자전 각도)
static std::vector<float> makeEllipse(
    float cx, float cy,
    float rx, float ry,
    int segments,
    float rotAngle = 0.0f)
{
    std::vector<float> verts;
    for (int i = 0; i <= segments; i++) {
        float a  = 2.0f * (float)M_PI * i / segments;
        float lx = rx * cosf(a);
        float ly = ry * sinf(a);
        // 2D 회전 행렬
        float rx2 = lx * cosf(rotAngle) - ly * sinf(rotAngle);
        float ry2 = lx * sinf(rotAngle) + ly * cosf(rotAngle);
        verts.push_back(cx + rx2);
        verts.push_back(cy + ry2);
        verts.push_back(0.0f);
    }
    return verts;
}

void drawRingPlanet() {

    const float cx  = -0.70f;
    const float cy  =  0.625f;
    const int   seg =  64;

    // ── 자체 시간 관리 (공통 파일 수정 없이) ─────────────────
    // 처음 호출될 때 시작 시각을 기록하고,
    // 이후 호출마다 경과 시간(초)을 계산해서 자전 각도에 사용
    static auto startTime = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    float elapsed = std::chrono::duration<float>(now - startTime).count();

    // 초당 0.5 라디안 → 약 12.5초에 한 바퀴 자전
    float rot = elapsed * 0.5f;

    // (1) 행성 본체: 원
    const float pR = 0.10f;
    drawLineLoop(makeEllipse(cx, cy, pR, pR, seg));

    // (2) 자전 효과: 적도선 (납작한 타원이 rot 각도로 기울어짐)
    drawLineLoop(makeEllipse(cx, cy, pR * 0.95f, pR * 0.25f, seg, rot));

    // (3) 자전 효과: 경도선 (90도 오프셋)
    drawLineLoop(makeEllipse(cx, cy, pR * 0.95f, pR * 0.25f, seg, rot + (float)M_PI * 0.5f));

    // (4) 고리 바깥 타원
    const float ringOutRx = 0.20f;
    const float ringOutRy = 0.045f;
    drawLineLoop(makeEllipse(cx, cy, ringOutRx, ringOutRy, seg));

    // (5) 고리 안쪽 타원 (두께감)
    const float ringInRx = 0.14f;
    const float ringInRy = 0.030f;
    drawLineLoop(makeEllipse(cx, cy, ringInRx, ringInRy, seg));

    // (6) 고리 앞쪽 반타원 (행성이 고리 뒤에 있는 느낌)
    {
        std::vector<float> front;
        for (int i = 0; i <= seg; i++) {
            float a = (float)M_PI * i / seg;
            front.push_back(cx + ringOutRx * cosf(a));
            front.push_back(cy - ringOutRy * sinf(a));
            front.push_back(0.0f);
        }
        drawLineStrip(front);
    }
    {
        std::vector<float> frontIn;
        for (int i = 0; i <= seg; i++) {
            float a = (float)M_PI * i / seg;
            frontIn.push_back(cx + ringInRx * cosf(a));
            frontIn.push_back(cy - ringInRy * sinf(a));
            frontIn.push_back(0.0f);
        }
        drawLineStrip(frontIn);
    }
}