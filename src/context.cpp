// pro_opengl_07 src/context.cpp

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

    // 배경색: 우주(어두운 남색)
    glClearColor(0.0f, 0.05f, 0.15f, 1.0f);

    return true;
}

void Context::Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_program->Get());

    // --- 우주 탐사 장면 오브젝트 렌더링 ---
    // 각 담당자가 자신의 cpp 파일에서 함수를 구현하면 여기서 자동 호출됩니다.

    drawRingPlanet();          // 고리 달린 행성 (feature/ring-planet)
    drawUFO();                 // UFO 우주선     (feature/ufo)
    drawRocket();              // 로켓           (feature/rocket)
    drawConstellationAndComet(); // 별자리 + 혜성  (feature/constellation-comet)
}