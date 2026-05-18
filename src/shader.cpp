// pro_opengl_06 src/shader.cpp

#include "shader.h"

ShaderUPtr Shader::CreateFromFile(const std::string& filename, GLenum shaderType){
    auto shader = ShaderUPtr(new Shader());

    if(!shader->LoadFile(filename, shaderType))
        return nullptr;

    return shader;
}

// 소멸자
Shader::~Shader() {
    if (m_shader != 0) {
        glDeleteShader(m_shader);
        m_shader = 0;
    }
}

bool Shader::LoadFile(const std::string& filename, GLenum shaderType){

    auto result = LoadTextFile(filename);
    if (!result)
        return false;

    const auto& code = result.value();
    const char* codePtr = code.c_str();
    int32_t codeLength = static_cast<int32_t>(code.length());

    m_shader = glCreateShader(shaderType);
    if(m_shader == 0) {
        SPDLOG_ERROR("glCreateShader failed");
        return false;
    }

// LoadFile() 구현 glShaderSource()로 소스코드 입력
glShaderSource(m_shader, 1, &codePtr, &codeLength);
// LoadFile() 구현 glcompileShader() shader로 컴파일
glCompileShader(m_shader);

// LoadFile() 구현
int success = 0;
glGetShaderiv(m_shader, GL_COMPILE_STATUS, &success);

if(!success) {
    int logLength = 0;
    glGetShaderiv(m_shader, GL_INFO_LOG_LENGTH, &logLength);

    std::string log(logLength, '\0');
    glGetShaderInfoLog(m_shader, logLength, nullptr, log.data());

    SPDLOG_ERROR("failed to compile shader: '{}'",filename);
    SPDLOG_ERROR("reason:\n{}", log);

    glDeleteShader(m_shader);
    m_shader = 0;
    return false;
}

return true;
}