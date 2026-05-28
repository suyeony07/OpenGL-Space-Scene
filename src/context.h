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
    float m_rocketX = -0.80f;
    float m_rocketY = -0.75f;
    float m_ufoX = 0.70f;
    float m_ufoY = 0.58f; 
    int m_gameState = 0;
    int m_lives = 2;       // 목숨 2개
    float m_hitTimer = 0.0f;  // 피격 시 무적 타이머
};

#endif // __CONTEXT_H__