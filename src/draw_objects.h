// src/draw_objects.h
// 우주 탐사 장면 - 각 오브젝트 드로우 함수 선언

#ifndef __DRAW_OBJECTS_H__
#define __DRAW_OBJECTS_H__

// 로켓 (담당 브랜치: feature/rocket)
// 좌표 범위: x[-0.25 ~ 0.25], y[-0.40 ~ 0.70]
void drawRocket();

// 고리 달린 행성 (담당 브랜치: feature/ring-planet)
// 좌표 범위: x[-0.95 ~ -0.45], y[0.35 ~ 0.90]
void drawRingPlanet();

// UFO 우주선 (담당 브랜치: feature/ufo)
// 좌표 범위: x[0.45 ~ 0.95], y[0.40 ~ 0.85]
void drawUFO();

// 별자리 + 혜성/운석 (담당 브랜치: feature/constellation-comet)
// 좌표 범위: x[-0.90 ~ 0.90], y[-0.85 ~ 0.30]
void drawConstellationAndComet();

#endif // __DRAW_OBJECTS_H__
