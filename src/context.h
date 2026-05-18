// pro_opengl_06 src/context.h

#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "common.h"
#include "shader.h"
#include "program.h"
#include "draw_objects.h"

// ... context 클래스 선언
CLASS_PTR(Context);

class Context {
    public:
    static ContextUPtr Create();

    ~Context() = default;

    void Render();

    Context(const Context&) = delete;
    Context& operator=(const Context&) = delete;

    private:
    Context() {}
    bool Init();

    ProgramUPtr m_program;
};

#endif // __CONTEXT_H__
