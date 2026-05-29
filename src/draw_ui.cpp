// src/draw_ui.cpp
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

static void drawFillEllipse(float cx, float cy, float rx, float ry, int seg) {
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

static void drawLineEllipse(float cx, float cy, float rx, float ry, int seg) {
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

static void drawTri(float x0, float y0, float x1, float y1, float x2, float y2) {
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
void drawHeart(float x, float y, float size, bool filled) {
    initShader();
    glUseProgram(s_prog);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float s = size;

    if (filled) {
        // 채워진 빨간 하트
        col(0.95f, 0.15f, 0.20f);
        drawFillEllipse(x - s*0.5f, y + s*0.3f, s*0.52f, s*0.45f, 20);
        drawFillEllipse(x + s*0.5f, y + s*0.3f, s*0.52f, s*0.45f, 20);
        drawTri(x - s*1.02f, y + s*0.15f,
                x + s*1.02f, y + s*0.15f,
                x, y - s*1.0f);
    } else {
        // 빈 하트 (외곽선만)
        col(0.50f, 0.20f, 0.22f);
        drawLineEllipse(x - s*0.5f, y + s*0.3f, s*0.52f, s*0.45f, 20);
        drawLineEllipse(x + s*0.5f, y + s*0.3f, s*0.52f, s*0.45f, 20);
        // 아래쪽 삼각형 외곽선
        float v[] = {
            x - s*1.02f, y + s*0.15f, 0,
            x + s*1.02f, y + s*0.15f, 0,
            x, y - s*1.0f, 0
        };
        GLuint vao, vbo;
        glGenVertexArrays(1, &vao); glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_LINE_LOOP, 0, 3);
        glDeleteBuffers(1, &vbo); glDeleteVertexArrays(1, &vao);
    }

    glDisable(GL_BLEND);
}
void drawGameUI(int lives, int gameState) {
    initShader();
    glUseProgram(s_prog);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 하트 2개 (왼쪽 상단)
    float heartY = 0.90f;
    float heartSize = 0.025f;
    drawHeart(-0.90f, heartY, heartSize, lives >= 1);
    drawHeart(-0.82f, heartY, heartSize, lives >= 2);

    // 게임오버 — 빨간 비네팅 (강하게)
    // 게임오버 — 얇은 빨간 테두리
    if (gameState == 2) {
        col(0.95f, 0.10f, 0.10f, 0.85f);
        float t = 0.015f;
        drawRect(-1.0f, -1.0f,  1.0f, -1.0f + t);
        drawRect(-1.0f,  1.0f - t, 1.0f, 1.0f);
        drawRect(-1.0f, -1.0f, -1.0f + t, 1.0f);
        drawRect( 1.0f - t, -1.0f, 1.0f, 1.0f);
    }

    // 클리어 — 얇은 황금 테두리
    if (gameState == 1) {
        col(1.0f, 0.85f, 0.20f, 0.90f);
        float t = 0.015f;
        drawRect(-1.0f, -1.0f,  1.0f, -1.0f + t);
        drawRect(-1.0f,  1.0f - t, 1.0f, 1.0f);
        drawRect(-1.0f, -1.0f, -1.0f + t, 1.0f);
        drawRect( 1.0f - t, -1.0f, 1.0f, 1.0f);
    }

    glDisable(GL_BLEND);
}
void drawParticle(float x, float y, float life) {
    initShader();
    glUseProgram(s_prog);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 파편 색상: 주황 → 노랑 → 투명
    float r = 1.0f;
    float g = 0.4f + life * 0.5f;
    float b = 0.1f * life;
    float a = life * 0.85f;
    float size = 0.012f * life + 0.003f;

    col(r, g, b, a);
    drawFillEllipse(x, y, size, size, 10);

    // 중심 밝은 점
    col(1.0f, 0.95f, 0.7f, a);
    drawFillEllipse(x, y, size*0.4f, size*0.4f, 8);

    glDisable(GL_BLEND);
}