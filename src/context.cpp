// src/context.cpp

#include "context.h"

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
    if (action != 1) return;  // GLFW_PRESS
    if (key >= '1' && key <= '4') {
        m_theme = key - '0';
        SPDLOG_INFO("Theme changed to {}", m_theme);
    }
}

void Context::Render() {
    // 테마별 배경색
    switch (m_theme) {
        case 1: glClearColor(0.00f, 0.05f, 0.15f, 1.0f); break;  // 기본 짙은 남색
        case 2: glClearColor(0.15f, 0.03f, 0.25f, 1.0f); break;  // 보라 성운
        case 3: glClearColor(0.00f, 0.00f, 0.00f, 1.0f); break;  // 칠흑 우주
        case 4: glClearColor(0.02f, 0.12f, 0.18f, 1.0f); break;  // 깊은 바다 청록    
    }
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_program->Get());

    drawStars();
    drawRingPlanet();
    drawUFO();
    drawRocket();
    drawConstellationAndComet();
}