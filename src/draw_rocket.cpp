// src/draw_rocket.cpp
// 담당 브랜치: feature/rocket
// 담당자가 이 파일만 수정하세요.
//
// ※ 이 파일은 로켓 전용 셰이더를 내부에 포함합니다.
//   simple.fs / simple.vs 를 전혀 수정하지 않으므로
//   다른 팀원 브랜치와 충돌이 발생하지 않습니다.

#include "draw_objects.h"
#include <glad/glad.h>
#include <cmath>
#include <vector>
#include <utility>

// ============================================================
//  로켓 전용 셰이더 소스 (문자열로 내장)
//  - 버텍스 셰이더: aPos(vec3) → gl_Position
//  - 프래그먼트 셰이더: uniform vec4 uColor → fragColor
// ============================================================
static const char* ROCKET_VS = R"glsl(
#version 330 core
layout(location = 0) in vec3 aPos;
void main() {
    gl_Position = vec4(aPos, 1.0);
}
)glsl";

static const char* ROCKET_FS = R"glsl(
#version 330 core
out vec4 fragColor;
uniform vec4 uColor = vec4(1.0, 1.0, 1.0, 1.0);
void main() {
    fragColor = uColor;
}
)glsl";

// ============================================================
//  로켓 전용 셰이더 프로그램 (최초 호출 시 한 번만 컴파일)
// ============================================================
static GLuint getRocketProgram() {
    static GLuint prog = 0;
    if (prog != 0) return prog;

    // 버텍스 셰이더 컴파일
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &ROCKET_VS, nullptr);
    glCompileShader(vs);

    // 프래그먼트 셰이더 컴파일
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &ROCKET_FS, nullptr);
    glCompileShader(fs);

    // 링크
    prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return prog;
}

// ============================================================
//  헬퍼: uColor uniform 설정
// ============================================================
static void setColor(GLuint prog, float r, float g, float b, float a = 1.0f) {
    GLint loc = glGetUniformLocation(prog, "uColor");
    if (loc != -1)
        glUniform4f(loc, r, g, b, a);
}

// ============================================================
//  헬퍼: 2D 정점 배열 → VAO/VBO 생성 → 그리기 → 즉시 해제
// ============================================================
static void drawShape(const float* vertices, int count, GLenum mode) {
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    float* buf = new float[count * 3];
    for (int i = 0; i < count; ++i) {
        buf[i * 3 + 0] = vertices[i * 2 + 0];
        buf[i * 3 + 1] = vertices[i * 2 + 1];
        buf[i * 3 + 2] = 0.0f;
    }
    glBufferData(GL_ARRAY_BUFFER, count * 3 * sizeof(float), buf, GL_DYNAMIC_DRAW);
    delete[] buf;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glDrawArrays(mode, 0, count);

    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

// ============================================================
//  헬퍼: 채워진 삼각형
// ============================================================
static void drawFilledTriangle(
    float x0, float y0,
    float x1, float y1,
    float x2, float y2)
{
    float v[] = { x0, y0, x1, y1, x2, y2 };
    drawShape(v, 3, GL_TRIANGLES);
}

// ============================================================
//  헬퍼: 타원 — 채우기(TRIANGLE_FAN) + 외곽선(LINE_LOOP)
// ============================================================
static void drawEllipse(GLuint prog,
    float cx, float cy, float rx, float ry,
    int segments,
    float fr, float fg, float fb,
    float lr, float lg, float lb)
{
    const float PI = 3.14159265f;
    std::vector<float> pts;

    // 채우기
    pts.push_back(cx); pts.push_back(cy);
    for (int i = 0; i <= segments; ++i) {
        float a = 2.0f * PI * i / segments;
        pts.push_back(cx + rx * cosf(a));
        pts.push_back(cy + ry * sinf(a));
    }
    setColor(prog, fr, fg, fb);
    drawShape(pts.data(), (int)(pts.size() / 2), GL_TRIANGLE_FAN);

    // 외곽선
    pts.clear();
    for (int i = 0; i < segments; ++i) {
        float a = 2.0f * PI * i / segments;
        pts.push_back(cx + rx * cosf(a));
        pts.push_back(cy + ry * sinf(a));
    }
    setColor(prog, lr, lg, lb);
    drawShape(pts.data(), segments, GL_LINE_LOOP);
}

// ============================================================
//  drawRocket()
//  좌표 범위: x[-0.25 ~ 0.25], y[-0.40 ~ 0.70]
//
//  구성:
//    1. 머리(노즈콘)  — 삼각형
//    2. 몸통          — 세로 타원 (채우기 + 외곽선)
//    3. 창문          — 채워진 원
//    4. 왼쪽 날개     — 삼각형
//    5. 오른쪽 날개   — 삼각형
//    6. 불꽃          — 채워진 삼각형 3개
// ============================================================
void drawRocket(float offsetX, float offsetY, float scale) {

    GLint prevProgram = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prevProgram);

    GLuint rocketProg = getRocketProgram();
    glUseProgram(rocketProg);

    auto sx = [&](float x) { return x * scale + offsetX; };
    auto sy = [&](float y) { return y * scale + offsetY; };

    // 6-1. 중앙 불꽃
    setColor(rocketProg, 1.0f, 0.9f, 0.1f);
    drawFilledTriangle(sx(-0.07f),sy(-0.15f), sx(0.07f),sy(-0.15f), sx(0.00f),sy(-0.40f));
    { float v[]={sx(-0.07f),sy(-0.15f), sx(0.07f),sy(-0.15f), sx(0.00f),sy(-0.40f)};
      setColor(rocketProg, 1.0f, 0.7f, 0.0f); drawShape(v,3,GL_LINE_LOOP); }

    setColor(rocketProg, 1.0f, 0.5f, 0.1f);
    drawFilledTriangle(sx(-0.10f),sy(-0.15f), sx(-0.04f),sy(-0.15f), sx(-0.07f),sy(-0.30f));

    setColor(rocketProg, 1.0f, 0.5f, 0.1f);
    drawFilledTriangle(sx(0.04f),sy(-0.15f), sx(0.10f),sy(-0.15f), sx(0.07f),sy(-0.30f));

    setColor(rocketProg, 1.0f, 0.6f, 0.2f);
    drawFilledTriangle(sx(-0.10f),sy(0.05f), sx(-0.10f),sy(-0.15f), sx(-0.25f),sy(-0.15f));
    { float v[]={sx(-0.10f),sy(0.05f), sx(-0.10f),sy(-0.15f), sx(-0.25f),sy(-0.15f)};
      setColor(rocketProg, 1.0f, 0.85f, 0.5f); drawShape(v,3,GL_LINE_LOOP); }

    setColor(rocketProg, 1.0f, 0.6f, 0.2f);
    drawFilledTriangle(sx(0.10f),sy(0.05f), sx(0.10f),sy(-0.15f), sx(0.25f),sy(-0.15f));
    { float v[]={sx(0.10f),sy(0.05f), sx(0.10f),sy(-0.15f), sx(0.25f),sy(-0.15f)};
      setColor(rocketProg, 1.0f, 0.85f, 0.5f); drawShape(v,3,GL_LINE_LOOP); }

    drawEllipse(rocketProg, sx(0.00f),sy(0.15f), 0.10f*scale,0.30f*scale, 48,
        0.3f,0.6f,0.9f, 0.75f,0.92f,1.0f);

    setColor(rocketProg, 0.4f, 0.7f, 1.0f);
    drawFilledTriangle(sx(-0.10f),sy(0.43f), sx(0.10f),sy(0.43f), sx(0.00f),sy(0.70f));
    { float v[]={sx(-0.10f),sy(0.43f), sx(0.10f),sy(0.43f), sx(0.00f),sy(0.70f)};
      setColor(rocketProg, 0.85f, 0.97f, 1.0f); drawShape(v,3,GL_LINE_LOOP); }

    drawEllipse(rocketProg, sx(0.00f),sy(0.22f), 0.055f*scale,0.055f*scale, 32,
        0.5f,0.85f,1.0f, 1.0f,1.0f,1.0f);
    drawEllipse(rocketProg, sx(-0.015f),sy(0.237f), 0.018f*scale,0.018f*scale, 16,
        1.0f,1.0f,1.0f, 1.0f,1.0f,1.0f);

    glUseProgram(prevProgram);
}