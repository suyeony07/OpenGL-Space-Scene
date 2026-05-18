// pro_opengl_06 program.cpp

#include "program.h"

ProgramUPtr Program::Create(const std::vector<ShaderPtr>& shaders) {
    auto program = ProgramUPtr(new Program());

    if (!program->Link(shaders))
        return nullptr;

    return program;
}

// 소멸자
Program::~Program() {
    if (m_program != 0) {
        glDeleteProgram(m_program);
        m_program = 0;
    }
}

bool Program::Link(const std::vector<ShaderPtr>& shaders) {

    GLuint program = glCreateProgram();
    if (!program) {
        SPDLOG_ERROR("glCreateProgram failed");
        return false;
    }

    for (const auto& shader : shaders)
        glAttachShader(program, shader->Get());
    glLinkProgram(program);

    int success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if(!success) {
        int logLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

        std::string log(logLength, '\0');
        glGetProgramInfoLog(program, logLength, nullptr, log.data());

        SPDLOG_ERROR("failed to loink program\n{}", log);

        glDeleteProgram(program);
        return false;
    }

    m_program = program;
    return true;
}