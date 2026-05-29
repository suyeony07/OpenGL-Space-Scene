// src/context.cpp

#include "context.h"
#include <GLFW/glfw3.h>
#include <cmath>

ContextUPtr Context::Create() {
    ContextUPtr context(new Context());

    if (!context->Init())
        return {};

    return context;
}

bool Context::Init() {

    ShaderPtr vertShader =
        Shader::CreateFromFile("./shader/simple.vs", GL_VERTEX_SHADER);

    ShaderPtr fragShader =
        Shader::CreateFromFile("./shader/simple.fs", GL_FRAGMENT_SHADER);

    if (!vertShader || !fragShader) {
        SPDLOG_ERROR("Failed to load shaders");
        return false;
    }

    m_program = Program::Create({ vertShader, fragShader });

    if (!m_program) {
        SPDLOG_ERROR("Failed to create program");
        return false;
    }

    SPDLOG_INFO("vertex shader id: {}", vertShader->Get());
    SPDLOG_INFO("fragment shader id: {}", fragShader->Get());
    SPDLOG_INFO("program id: {}", m_program->Get());

    return true;
}

void Context::ProcessInput(int key, int action) {
    if (action != GLFW_PRESS && action != GLFW_REPEAT) return;

   if (key == GLFW_KEY_0 && action == GLFW_PRESS) {
        m_rocketX = -0.80f;
        m_rocketY = -0.75f;
        m_ufoX = 0.70f;
        m_ufoY = 0.58f;
        m_gameState = 0;
        m_lives = 2;
        m_hitTimer = 0.0f;
        m_numParticles = 0;  // 파편 초기화
        SPDLOG_INFO("Game reset!");
        return;
    }

    if (key >= GLFW_KEY_1 && key <= GLFW_KEY_4) {
        m_theme = key - GLFW_KEY_0;
        SPDLOG_INFO("Theme changed to {}", m_theme);
    }

    if (m_gameState == 0) {
        float speed = 0.02f;
        if (key == GLFW_KEY_UP)    m_rocketY += speed;
        if (key == GLFW_KEY_DOWN)  m_rocketY -= speed;
        if (key == GLFW_KEY_LEFT)  m_rocketX -= speed;
        if (key == GLFW_KEY_RIGHT) m_rocketX += speed;

        SPDLOG_INFO("Rocket pos: ({}, {})", m_rocketX, m_rocketY);

        if (m_rocketX < -0.85f) m_rocketX = -0.85f;
        if (m_rocketX >  0.85f) m_rocketX =  0.85f;
        if (m_rocketY < -0.85f) m_rocketY = -0.85f;
        if (m_rocketY >  0.85f) m_rocketY =  0.85f;
    }
}

void Context::Render() {
    switch (m_theme) {
        case 1: glClearColor(0.00f, 0.05f, 0.15f, 1.0f); break;
        case 2: glClearColor(0.15f, 0.03f, 0.25f, 1.0f); break;
        case 3: glClearColor(0.00f, 0.00f, 0.00f, 1.0f); break;
        case 4: glClearColor(0.02f, 0.12f, 0.18f, 1.0f); break;
    }
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(m_program->Get());

    float time = (float)glfwGetTime();

    drawStars();
    drawRingPlanet();

    // UFO 추적
    if (m_gameState == 0) {
        float dx = m_rocketX - m_ufoX;
        float dy = m_rocketY - m_ufoY;
        float dist = sqrtf(dx*dx + dy*dy);
        if (dist > 0.01f) {
            float ufoSpeed = 0.003f;
            m_ufoX += (dx / dist) * ufoSpeed;
            m_ufoY += (dy / dist) * ufoSpeed;
        }
    }

    drawUFO(m_ufoX, m_ufoY);
    drawObstacles(time);

    float moonX = 0.70f, moonY = -0.65f;
    drawMoon(moonX, moonY, time);

    bool visible = true;
    if (m_hitTimer > 0.0f) {
        m_hitTimer -= 0.016f;
        visible = ((int)(m_hitTimer * 15.0f) % 2 == 0);
    }
    if (m_gameState == 2) {
        visible = ((int)(time * 8.0f) % 2 == 0);
    }
    if (m_gameState == 1) {
        m_rocketX = moonX;
        m_rocketY = moonY + 0.12f;
    }
    if (visible) {
        drawRocket(m_rocketX, m_rocketY, 0.4f);
    }

    drawConstellationAndComet();

    // ── 파편 업데이트 + 그리기 ───────────────────
    for (int i = 0; i < m_numParticles; ) {
        auto& p = m_particles[i];
        p.x += p.vx * 0.016f;
        p.y += p.vy * 0.016f;
        p.vy -= 0.5f * 0.016f;  // 중력
        p.life -= 0.025f;

        if (p.life <= 0.0f) {
            // 마지막 파편을 현재 위치로 옮기고 개수 감소
            m_particles[i] = m_particles[m_numParticles - 1];
            m_numParticles--;
        } else {
            drawParticle(p.x, p.y, p.life);
            i++;
        }
    }

    drawGameUI(m_lives, m_gameState);

    // ── 충돌 판정 ────────────────────────────────
    if (m_gameState == 0 && m_hitTimer <= 0.0f) {

        // UFO 충돌
        float udx = m_rocketX - m_ufoX;
        float udy = m_rocketY - m_ufoY;
        float udist = sqrtf(udx*udx + udy*udy);
        if (udist < 0.15f) {
            m_lives--;
            // 파편 생성 (로켓 위치)
            for (int i = 0; i < 8 && m_numParticles < 20; i++) {
                float ang = (float)i / 8 * 6.28f + (time * 3.0f);
                m_particles[m_numParticles++] = {
                    m_rocketX, m_rocketY,
                    cosf(ang) * 0.8f, sinf(ang) * 0.8f,
                    1.0f
                };
            }
            if (m_lives <= 0) {
                m_gameState = 2;
                SPDLOG_INFO("GAME OVER!");
            } else {
                m_hitTimer = 1.5f;
                SPDLOG_INFO("UFO HIT! Lives: {}", m_lives);
            }
        }

        // 소행성 충돌
        struct AstData { float x; float speed; float offset; float size; };
        AstData asts[] = {
            {-0.70f,0.38f,0.0f,0.05f},
            {-0.45f,0.55f,1.2f,0.06f},
            {-0.20f,0.48f,2.5f,0.05f},
            { 0.00f,0.62f,0.5f,0.06f},
            { 0.20f,0.42f,3.5f,0.05f},
            { 0.40f,0.50f,1.8f,0.06f},
            { 0.60f,0.58f,2.8f,0.05f},
            {-0.35f,0.80f,4.0f,0.035f},
            { 0.15f,0.75f,5.2f,0.035f},
            { 0.50f,0.72f,3.8f,0.035f},
        };
        for (auto& a : asts) {
            float ay = 1.2f - fmodf((time + a.offset) * a.speed, 2.4f);
            float dx = m_rocketX - a.x;
            float dy = m_rocketY - ay;
            float dist = sqrtf(dx*dx + dy*dy);
            if (dist < a.size + 0.03f) {
                m_lives--;
                // 파편 생성 (충돌 지점)
                float hitX = (m_rocketX + a.x) * 0.5f;
                float hitY = (m_rocketY + ay) * 0.5f;
                for (int i = 0; i < 10 && m_numParticles < 20; i++) {
                    float ang = (float)i / 10 * 6.28f + (time * 5.0f);
                    float spd = 0.6f + (float)(i % 3) * 0.3f;
                    m_particles[m_numParticles++] = {
                        hitX, hitY,
                        cosf(ang) * spd, sinf(ang) * spd + 0.3f,
                        1.0f
                    };
                }
                if (m_lives <= 0) {
                    m_gameState = 2;
                    SPDLOG_INFO("GAME OVER!");
                } else {
                    m_hitTimer = 1.5f;
                    SPDLOG_INFO("HIT! Lives: {}", m_lives);
                }
                break;
            }
        }

        // 달 도착
        float dx = m_rocketX - moonX;
        float dy = m_rocketY - moonY;
        if (sqrtf(dx*dx + dy*dy) < 0.16f) {
            m_gameState = 1;
            SPDLOG_INFO("CLEAR!");
        }
    }
}