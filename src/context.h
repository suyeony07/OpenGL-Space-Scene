#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "common.h"
#include "shader.h"
#include "program.h"
#include "draw_objects.h"

CLASS_PTR(Context);

class Context {
public:
    static ContextUPtr Create();
    ~Context() = default;
    void Render();
    void ProcessInput(int key, int action);

    Context(const Context&) = delete;
    Context& operator=(const Context&) = delete;

private:
    Context() {}
    bool Init();

    ProgramUPtr m_program;
    int m_theme = 1;  // 1~4 테마
};

#endif // __CONTEXT_H__