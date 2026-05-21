// src/draw_constellation_comet.cpp
// 담당 브랜치: feature/constellation-comet
// 담당자가 이 파일만 수정하세요.

#include "draw_objects.h"
#include <glad/glad.h>

void drawConstellationAndComet() {
    // TODO: 별자리 + 혜성/운석 담당자가 feature/constellation-comet 브랜치에서 구현
    //
    // 좌표 범위:
    //   x: -0.90 ~ 0.90
    //   y: -0.85 ~ 0.30
    //
    // 구현 가이드:
    //   - OpenGL 선 그리기(GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_POINTS)를 활용
    //   - 별자리: 별(점) + 연결선(GL_LINES)으로 표현
    //   - 혜성/운석: 이동 궤적 및 꼬리를 선으로 표현

    // -------------------------------------------------------------------------
    // 1. 카시오페아 별자리 (Cassiopeia) 구현
    // -------------------------------------------------------------------------
    // W 모양을 이루는 5개의 주요 별 좌표 (x: -0.90 ~ 0.90, y: -0.85 ~ 0.30 범위 내)
    float stars[5][2] = {
        {-0.65f,  0.15f},  // ε (에프실론)
        {-0.45f, -0.10f},  // δ (델타)
        {-0.25f,  0.10f},  // γ (감마)
        {-0.05f, -0.15f},  // α (알파)
        { 0.15f,  0.05f}   // β (베타)
    };

    // [별자리의 연결선 그리기]
    // 선의 색상: 은은하고 신비로운 청백색 (White-Blue)
    glColor3f(0.5f, 0.7f, 0.9f); 
    glLineWidth(1.5f); // 선 두께 설정

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < 5; ++i) {
        glVertex2f(stars[i][0], stars[i][1]);
    }
    glEnd();

    // [별자리 점(별) 그리기]
    // 별의 색상: 밝게 빛나는 노란빛 흰색
    glColor3f(1.0f, 1.0f, 0.85f);
    glPointSize(6.0f); // 별이 잘 보이도록 점 크기 키움

    glBegin(GL_POINTS);
    for (int i = 0; i < 5; ++i) {
        glVertex2f(stars[i][0], stars[i][1]);
    }
    glEnd();


    // -------------------------------------------------------------------------
    // 2. 혜성과 운석 파편 (Comet & Meteors) 구현
    // -------------------------------------------------------------------------
    // 혜성의 머리(핵) 위치: 우상단에서 좌하단으로 떨어지는 느낌으로 배치
    float cometHeadX = 0.70f;
    float cometHeadY = 0.20f;
    
    // 혜성의 이동 방향 벡터 (꼬리가 뻗어나갈 반대 방향 계산용)
    float dirX = -0.45f; 
    float dirY = -0.35f;

    // [혜성의 꼬리(Tail) 그리기]
    // 여러 개의 선을 겹쳐 그려 뒤로 갈수록 우주 배경 속으로 흐려지는 효과 연출
    glLineWidth(2.5f);
    glBegin(GL_LINES);
    
    // 첫 번째 메인 꼬리 (가장 길고 밝음)
    glColor3f(0.4f, 0.8f, 1.0f); // 푸른빛 혜성 꼬리
    glVertex2f(cometHeadX, cometHeadY);
    glColor3f(0.0f, 0.0f, 0.1f); // 끝으로 갈수록 어두워짐
    glVertex2f(cometHeadX - dirX * 0.9f, cometHeadY - dirY * 0.9f);

    // 두 번째 갈라지는 꼬리 (위쪽으로 살짝 퍼짐)
    glColor3f(0.3f, 0.6f, 0.9f);
    glVertex2f(cometHeadX, cometHeadY);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(cometHeadX - dirX * 0.7f + 0.04f, cometHeadY - dirY * 0.7f + 0.06f);

    // 세 번째 갈라지는 꼬리 (아래쪽으로 살짝 퍼짐)
    glColor3f(0.3f, 0.6f, 0.9f);
    glVertex2f(cometHeadX, cometHeadY);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(cometHeadX - dirX * 0.7f - 0.06f, cometHeadY - dirY * 0.7f - 0.04f);
    
    glEnd();

    // [혜성 주변에 부서져 내리는 운석 파편들]
    glPointSize(3.0f);
    glColor3f(0.7f, 0.9f, 1.0f);
    glBegin(GL_POINTS);
    glVertex2f(cometHeadX - 0.15f, cometHeadY - 0.10f);
    glVertex2f(cometHeadX - 0.25f, cometHeadY - 0.22f);
    glVertex2f(cometHeadX - 0.30f, cometHeadY - 0.20f);
    glVertex2f(cometHeadX - 0.45f, cometHeadY - 0.38f);
    glEnd();

    // [혜성의 머리(핵) 그리기]
    glColor3f(1.0f, 1.0f, 1.0f); // 순백색 중심점
    glPointSize(8.0f);
    
    glBegin(GL_POINTS);
    glVertex2f(cometHeadX, cometHeadY);
    glEnd();
    
    // -------------------------------------------------------------------------
    // 3. OpenGL 그래픽 상태 초기화
    // -------------------------------------------------------------------------
    // 다른 파일(로켓, 행성 등)을 그릴 때 선 두께나 점 크기가 꼬이지 않도록 초기화
    glLineWidth(1.0f);
    glPointSize(1.0f);
}
