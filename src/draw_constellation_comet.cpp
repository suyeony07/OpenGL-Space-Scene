// src/draw_constellation_comet.cpp
// 담당 브랜치: feature/constellation-comet
// 구현: 처녀자리 별자리 + 혜성 + 유성우
//
// 전체 좌표 범위: x[-0.90 ~ 0.90], y[-0.85 ~ 0.30]

#include "draw_objects.h"
#include <glad/glad.h>

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

    // =========================================================
    // 1. 처녀자리 (Virgo) 별자리
    //    주요 별 9개 + 연결선
    //
    //    실제 처녀자리 형태 (Y자 + 꼬리 형태):
    //
    //    Vindemiatrix(ε)         Zavijava(β)
    //         \                   /
    //          \                 /
    //        Porrima(γ) ---- Zaniah(η)
    //             |
    //          Spica(α)  ← 가장 밝은 별 (중심)
    //           /    \
    //      Heze(ζ)   Syrma(ι)
    //         |
    //      Minelava(δ)
    //
    //    x: -0.85 ~ 0.10, y: -0.20 ~ 0.28  (왼쪽 상단 영역)
    // =========================================================

    // 별 좌표
    static const float starPos[] = {
        -0.72f,  0.26f, 0.0f,   // ε Vindemiatrix (왼쪽 위)
        -0.20f,  0.24f, 0.0f,   // β Zavijava     (오른쪽 위)
        -0.48f,  0.10f, 0.0f,   // γ Porrima      (중상)
        -0.22f,  0.10f, 0.0f,   // η Zaniah       (중상 오른쪽)
        -0.48f, -0.08f, 0.0f,   // α Spica        (중심, 가장 밝음)
        -0.65f, -0.20f, 0.0f,   // ζ Heze         (왼쪽 아래)
        -0.35f, -0.20f, 0.0f,   // ι Syrma        (오른쪽 아래)
        -0.65f, -0.38f, 0.0f,   // δ Minelava     (왼쪽 하단)
        -0.82f,  0.10f, 0.0f,   // μ Rijl al Awwa (맨 왼쪽)
    };

    // 연결선
    static const float constellationLines[] = {
        // 위쪽 Y자 가지
        -0.72f,  0.26f, 0.0f,   -0.48f,  0.10f, 0.0f,   // ε→γ
        -0.20f,  0.24f, 0.0f,   -0.22f,  0.10f, 0.0f,   // β→η
        -0.22f,  0.10f, 0.0f,   -0.48f,  0.10f, 0.0f,   // η→γ
        // 중심 수직선
        -0.48f,  0.10f, 0.0f,   -0.48f, -0.08f, 0.0f,   // γ→α(Spica)
        // 아래쪽 갈래
        -0.48f, -0.08f, 0.0f,   -0.65f, -0.20f, 0.0f,   // α→ζ
        -0.48f, -0.08f, 0.0f,   -0.35f, -0.20f, 0.0f,   // α→ι
        // 꼬리
        -0.65f, -0.20f, 0.0f,   -0.65f, -0.38f, 0.0f,   // ζ→δ
        // 왼쪽 팔
        -0.82f,  0.10f, 0.0f,   -0.72f,  0.26f, 0.0f,   // μ→ε
    };

    glLineWidth(1.4f);
    drawPrimitive(constellationLines, sizeof(constellationLines) / sizeof(float), GL_LINES);

    // 일반 별 (작게)
    glPointSize(5.0f);
    drawPrimitive(starPos, sizeof(starPos) / sizeof(float), GL_POINTS);

    // Spica(α) 강조 - 처녀자리에서 가장 밝은 별
    static const float spica[] = { -0.48f, -0.08f, 0.0f };
    glPointSize(10.0f);
    drawPrimitive(spica, sizeof(spica) / sizeof(float), GL_POINTS);

    // =========================================================
    // 2. 배경 별 - y[-0.85~0.30] 전체에 고르게
    // =========================================================
    static const float bgStars[] = {
        // 상단 오른쪽 (별자리 없는 영역)
         0.15f,  0.28f, 0.0f,
         0.48f,  0.20f, 0.0f,
         0.75f,  0.28f, 0.0f,
         0.88f,  0.12f, 0.0f,
        // 중상
        -0.88f, -0.02f, 0.0f,
         0.05f,  0.05f, 0.0f,
         0.35f, -0.05f, 0.0f,
         0.72f, -0.10f, 0.0f,
         0.88f, -0.18f, 0.0f,
        // 중단
        -0.85f, -0.28f, 0.0f,
        -0.42f, -0.32f, 0.0f,
         0.10f, -0.25f, 0.0f,
         0.45f, -0.35f, 0.0f,
         0.82f, -0.30f, 0.0f,
        // 중하
        -0.78f, -0.52f, 0.0f,
        -0.28f, -0.48f, 0.0f,
         0.18f, -0.55f, 0.0f,
         0.55f, -0.50f, 0.0f,
         0.85f, -0.58f, 0.0f,
        // 하단
        -0.62f, -0.68f, 0.0f,
        -0.18f, -0.75f, 0.0f,
         0.22f, -0.72f, 0.0f,
         0.65f, -0.78f, 0.0f,
         0.88f, -0.82f, 0.0f,
    };

    glPointSize(2.5f);
    drawPrimitive(bgStars, sizeof(bgStars) / sizeof(float), GL_POINTS);

    // =========================================================
    // 3. 혜성 (Comet) - 화면 오른쪽 상단 영역
    //    핵: (0.45, 0.05)
    //    이동 방향: 오른쪽위 → 왼쪽아래
    //    꼬리: 핵 기준 오른쪽+위 방향으로 퍼짐
    //
    //    꼬리를 아름답게 표현:
    //      - 중심 가닥 1개 (가장 길고 밝음)
    //      - 퍼지는 가닥 6개 (위아래로 점점 짧아짐)
    //      - 꼬리 안쪽 보조 점들 (빛나는 느낌)
    // =========================================================

    // 핵 (크고 밝게)
    static const float cometHead[] = { 0.45f, 0.05f, 0.0f };
    glPointSize(12.0f);
    drawPrimitive(cometHead, sizeof(cometHead) / sizeof(float), GL_POINTS);

    // 핵 내부 밝은 중심
    static const float cometCore[] = { 0.45f, 0.05f, 0.0f };
    glPointSize(6.0f);
    drawPrimitive(cometCore, sizeof(cometCore) / sizeof(float), GL_POINTS);

    // 광환 (핵 주변 8방향 점)
    static const float halo[] = {
        0.49f,  0.09f, 0.0f,   // 우상
        0.41f,  0.09f, 0.0f,   // 좌상
        0.49f,  0.01f, 0.0f,   // 우하
        0.41f,  0.01f, 0.0f,   // 좌하
        0.45f,  0.11f, 0.0f,   // 위
        0.45f, -0.01f, 0.0f,   // 아래
        0.51f,  0.05f, 0.0f,   // 오른쪽
        0.39f,  0.05f, 0.0f,   // 왼쪽 (꼬리 시작)
    };
    glPointSize(3.5f);
    drawPrimitive(halo, sizeof(halo) / sizeof(float), GL_POINTS);

    // 꼬리 가닥: 핵(0.45, 0.05) → 오른쪽+위로 퍼짐
    // 중심에서 위아래로 부채꼴 형태
    // 끝점 x ≤ 0.90, y ≤ 0.28
    static const float tailVerts[] = {
        // 중심 가닥 (가장 길게)
        0.45f,  0.05f, 0.0f,    0.89f,  0.05f, 0.0f,
        // 위 +1
        0.45f,  0.05f, 0.0f,    0.88f,  0.14f, 0.0f,
        // 위 +2
        0.45f,  0.05f, 0.0f,    0.85f,  0.22f, 0.0f,
        // 위 +3 (짧음)
        0.45f,  0.05f, 0.0f,    0.78f,  0.28f, 0.0f,
        // 아래 -1
        0.45f,  0.05f, 0.0f,    0.88f, -0.04f, 0.0f,
        // 아래 -2
        0.45f,  0.05f, 0.0f,    0.85f, -0.12f, 0.0f,
        // 아래 -3 (짧음)
        0.45f,  0.05f, 0.0f,    0.78f, -0.18f, 0.0f,
    };

    glLineWidth(1.8f);
    drawPrimitive(tailVerts, sizeof(tailVerts) / sizeof(float), GL_LINES);

    // 꼬리 안쪽 빛나는 점들 (중심 가닥 위에 겹쳐서 밀도감)
    static const float tailGlow[] = {
        0.55f,  0.05f, 0.0f,
        0.62f,  0.06f, 0.0f,
        0.70f,  0.06f, 0.0f,
        0.78f,  0.06f, 0.0f,
    };
    glPointSize(2.5f);
    drawPrimitive(tailGlow, sizeof(tailGlow) / sizeof(float), GL_POINTS);

    // 궤적: 핵에서 왼쪽+아래 방향 (지나온 흔적, 점점 작아짐)
    static const float trailDots[] = {
        0.33f, -0.06f, 0.0f,
        0.22f, -0.16f, 0.0f,
        0.12f, -0.26f, 0.0f,
        0.02f, -0.35f, 0.0f,
       -0.07f, -0.43f, 0.0f,
    };
    glPointSize(3.0f);
    drawPrimitive(trailDots, sizeof(trailDots) / sizeof(float), GL_POINTS);

    // 궤적 보조 (더 작은 점)
    static const float trailFade[] = {
        0.28f, -0.11f, 0.0f,
        0.17f, -0.21f, 0.0f,
        0.07f, -0.30f, 0.0f,
    };
    glPointSize(1.8f);
    drawPrimitive(trailFade, sizeof(trailFade) / sizeof(float), GL_POINTS);

    // =========================================================
    // 4. 유성우 (Meteor Shower) - 하단 전체에 고르게
    //    짧고 일정한 선분 6개, 굵기 얇게 (혜성과 구분)
    //    방향: 오른쪽위→왼쪽아래 (45도)
    // =========================================================
    static const float meteors[] = {
        // 왼쪽 하단
        -0.72f, -0.42f, 0.0f,   -0.84f, -0.54f, 0.0f,
        -0.75f, -0.65f, 0.0f,   -0.87f, -0.77f, 0.0f,
        // 중앙 하단
        -0.10f, -0.45f, 0.0f,   -0.22f, -0.57f, 0.0f,
        -0.08f, -0.68f, 0.0f,   -0.20f, -0.80f, 0.0f,
        // 오른쪽 하단
         0.62f, -0.42f, 0.0f,    0.50f, -0.54f, 0.0f,
         0.60f, -0.65f, 0.0f,    0.48f, -0.77f, 0.0f,
    };

    glLineWidth(1.0f);
    drawPrimitive(meteors, sizeof(meteors) / sizeof(float), GL_LINES);

    // 유성 머리
    static const float meteorHeads[] = {
        -0.72f, -0.42f, 0.0f,
        -0.75f, -0.65f, 0.0f,
        -0.10f, -0.45f, 0.0f,
        -0.08f, -0.68f, 0.0f,
         0.62f, -0.42f, 0.0f,
         0.60f, -0.65f, 0.0f,
    };
    glPointSize(4.0f);
    drawPrimitive(meteorHeads, sizeof(meteorHeads) / sizeof(float), GL_POINTS);

    // 상태 복원
    glPointSize(1.0f);
    glLineWidth(1.0f);
}