// pro_opengl_06 program.h

#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include "common.h"
#include "shader.h"

// ... 클래스 선언
CLASS_PTR(Program);

class Program {
    public: 
    static ProgramUPtr Create(const std::vector<ShaderPtr>& shaders);

    ~Program();

    GLuint Get() const { return m_program; }

    private:
    Program() {}
    bool Link(const std::vector<ShaderPtr>& shaders);

    GLuint m_program = 0;
};

#endif // __PROGRAM_H__s