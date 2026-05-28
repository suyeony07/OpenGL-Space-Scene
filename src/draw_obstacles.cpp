// src/draw_obstacles.cpp
#include "draw_objects.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
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

static void drawVerts(const std::vector<float>& v, GLenum mode) {
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

// 울퉁불퉁한 소행성 모양 생성
static std::vector<float> makeRock(float cx, float cy, float size,
                                    const float* bumps, int numPts) {
    std::vector<float> v;
    v.push_back(cx); v.push_back(cy); v.push_back(0.f);
    float PI = 3.14159265f;
    for (int i = 0; i <= numPts; i++) {
        int idx = i % numPts;
        float a = 2.f * PI * i / numPts;
        float r = size * bumps[idx];
        v.push_back(cx + r * cosf(a));
        v.push_back(cy + r * sinf(a));
        v.push_back(0.f);
    }
    return v;
}

static std::vector<float> makeRockLine(float cx, float cy, float size,
                                        const float* bumps, int numPts) {
    std::vector<float> v;
    float PI = 3.14159265f;
    for (int i = 0; i < numPts; i++) {
        float a = 2.f * PI * i / numPts;
        float r = size * bumps[i];
        v.push_back(cx + r * cosf(a));
        v.push_back(cy + r * sinf(a));
        v.push_back(0.f);
    }
    return v;
}

// 4종류 소행성 형태 (울퉁불퉁 정도가 다름)
static const float shape0[] = { 1.0f,0.85f,1.1f,0.78f,0.95f,1.15f,0.82f,1.05f,0.90f,1.08f };
static const float shape1[] = { 0.90f,1.12f,0.80f,1.05f,0.88f,1.18f,0.75f,1.0f };
static const float shape2[] = { 1.05f,0.78f,1.15f,0.85f,0.92f,1.10f,0.80f,1.0f,0.88f,1.12f,0.95f,0.82f };
static const float shape3[] = { 0.95f,1.08f,0.82f,1.15f,0.90f,0.78f,1.05f,0.88f,1.12f };

struct AsteroidType {
    const float* bumps;
    int numPts;
    float bodyR, bodyG, bodyB;     // 본체
    float highR, highG, highB;     // 하이라이트
    float lineR, lineG, lineB;     // 외곽선
};

static const AsteroidType types[] = {
    // 짙은 회갈색
    { shape0, 10, 0.40f,0.32f,0.25f, 0.58f,0.50f,0.40f, 0.25f,0.18f,0.12f },
    // 청회색 (얼음 소행성)
    { shape1, 8,  0.38f,0.45f,0.55f, 0.60f,0.72f,0.82f, 0.20f,0.28f,0.38f },
    // 적갈색 (철 소행성)
    { shape2, 12, 0.50f,0.30f,0.22f, 0.70f,0.48f,0.35f, 0.30f,0.15f,0.10f },
    // 밝은 회색
    { shape3, 9,  0.52f,0.50f,0.48f, 0.72f,0.70f,0.68f, 0.32f,0.30f,0.28f },
};

struct Asteroid {
    float x;
    float speed;
    float offset;
    float size;
    int type;       // 0~3
    float rot;      // 회전 속도
};

static const Asteroid asteroids[] = {
    { -0.70f, 0.38f, 0.0f, 0.05f, 0,  1.2f },
    { -0.45f, 0.55f, 1.2f, 0.06f, 1, -0.9f },
    { -0.20f, 0.48f, 2.5f, 0.05f, 2,  1.4f },
    {  0.00f, 0.62f, 0.5f, 0.06f, 3, -1.0f },
    {  0.20f, 0.42f, 3.5f, 0.05f, 0,  1.1f },
    {  0.40f, 0.50f, 1.8f, 0.06f, 1, -0.8f },
    {  0.60f, 0.58f, 2.8f, 0.05f, 2,  1.3f },
    { -0.35f, 0.80f, 4.0f, 0.035f, 3,  1.8f },
    {  0.15f, 0.75f, 5.2f, 0.035f, 0, -1.6f },
    {  0.50f, 0.72f, 3.8f, 0.035f, 1,  2.0f },
};
static const int NUM_ASTEROIDS = 10;

void drawObstacles(float time) {
    initShader();
    glUseProgram(s_prog);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (int i = 0; i < NUM_ASTEROIDS; i++) {
        const auto& a = asteroids[i];
        const auto& t = types[a.type];

        float period = 2.4f / a.speed;
        float y = 1.2f - fmodf((time + a.offset) * a.speed, 2.4f);

        // 회전 적용된 bump 배열
        float rotAngle = time * a.rot;
        std::vector<float> rotBumps(t.numPts);
        for (int j = 0; j < t.numPts; j++) {
            int shifted = ((int)(j + rotAngle * 2) % t.numPts + t.numPts) % t.numPts;
            rotBumps[j] = t.bumps[shifted];
        }

        // 본체
        col(t.bodyR, t.bodyG, t.bodyB);
        drawVerts(makeRock(a.x, y, a.size, rotBumps.data(), t.numPts), GL_TRIANGLE_FAN);

        // 하이라이트 (왼쪽 위)
        col(t.highR, t.highG, t.highB, 0.55f);
        drawVerts(makeRock(a.x - a.size*0.15f, y + a.size*0.2f,
                           a.size*0.45f, rotBumps.data(), t.numPts), GL_TRIANGLE_FAN);

        // 외곽선
        col(t.lineR, t.lineG, t.lineB);
        drawVerts(makeRockLine(a.x, y, a.size, rotBumps.data(), t.numPts), GL_LINE_LOOP);

        // 크레이터 1
        col(t.lineR, t.lineG, t.lineB, 0.50f);
        drawVerts(makeRock(a.x + a.size*0.25f, y - a.size*0.15f,
                           a.size*0.18f, rotBumps.data(), t.numPts), GL_TRIANGLE_FAN);

        // 크레이터 2 (작은)
        col(t.lineR, t.lineG, t.lineB, 0.35f);
        drawVerts(makeRock(a.x - a.size*0.2f, y - a.size*0.25f,
                           a.size*0.12f, rotBumps.data(), t.numPts), GL_TRIANGLE_FAN);
    }

    glDisable(GL_BLEND);
}