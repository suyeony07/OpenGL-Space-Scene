// pro_opengl_06 src/shader.h

#ifndef __SHADER_H__
#define __SHADER_H__

#include "common.h"

// .... 본문
CLASS_PTR(Shader);

class Shader {
    public:
    static ShaderUPtr CreateFromFile(const std::string& filename, GLenum shaderType);

    ~Shader();

    GLuint Get() const { return m_shader; }

    private:
    Shader() {}
        bool LoadFile(const std::string& filename, GLenum shaderType);
        GLuint m_shader = 0;  
};

#endif // __SHADER_H__