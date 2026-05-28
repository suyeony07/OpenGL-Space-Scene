// pro_opengl_06 src/main.cpp

/*
#include "common.h"
#include "shader.h"
#include "program.h"
*/

#include "context.h"

#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// GLFW CALLBAVKS
void OnFramebufferSizeChange(GLFWwindow* window, int width, int height){
    SPDLOG_INFO("Framebuffer size changed: ({} X {})", width, height);
    glViewport(0, 0, width, height);
}

void OnKeyEvent(GLFWwindow* window,
        int key,
        int scancode,
        int action,
        int mods) {
            SPDLOG_INFO(
                "Key: {}, scancode: {}, action: {}, mods: {}{}{}",
                key,
                scancode,
                action == GLFW_PRESS    ? "Pressed" :
                action == GLFW_RELEASE  ? "Release" :
                action == GLFW_REPEAT   ? "Repeat" : "Unknown",
                mods & GLFW_MOD_CONTROL ? "C" : "_",
                mods & GLFW_MOD_SHIFT   ? "S" : "_",
                mods & GLFW_MOD_ALT     ? "A" : "_"
            );

            // ESC 키 누르면 종료
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
                glfwSetWindowShouldClose(window, true);
            }
        }


int main(int argc, const char** argv){
    SPDLOG_INFO("start program");

    // glfw 라이브러리 초기화, 실패하면 에러 출력 후 종료
    SPDLOG_INFO("Initialize glfw");
    if(!glfwInit()) {
        const char* description;
        glfwGetError(&description);
        SPDLOG_ERROR("failed to Initialize glfw: {}", description);
   return -1;
    }

// ... glfwInit() 호출 후

glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 

// ... glfwCreateWindow() 호출 전

// glfw 윈도우 생성, 실패하면 에러 출력 후 종료
SPDLOG_INFO("Create glfw window");
auto window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
if(!window) {
    SPDLOG_ERROR("failed to Initialize glfw window");
    glfwTerminate();

    return -1;
}

// 윈도우 생성 후 OpenGL Context를 사용
glfwMakeContextCurrent(window);

// glad를 활용한 OpenGL 함수 로딩
if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        SPDLOG_ERROR("failed to Initialize glad");
        glfwTerminate();
        return -1;
}

// GLAD 초기화
auto glVersion = glGetString(GL_VERSION);
SPDLOG_INFO("OpenGL context version: {}", (const char*)glVersion);

/* 삭제
auto program = Program::Create({ vertShader, fragShader });

SPDLOG_INFO("vertex shader id: {}", vertexShader->Get());
SPDLOG_INFO("fragment shader id: {}", fragmentShader->Get());
SPDLOG_INFO("program id: {}", m_program->Get());
*/

auto context = Context::Create();
if (!context) {
    SPDLOG_ERROR("failed to create context");
    glfwTerminate();
    return -1;
}

// GLFW CALLBACKS
OnFramebufferSizeChange(window, WINDOW_WIDTH, WINDOW_HEIGHT);
// GLFW CALLBAVKS(2) 함께 추가
glfwSetFramebufferSizeCallback(window, OnFramebufferSizeChange);
// GLFW CALLBAVKS
glfwSetKeyCallback(window, OnKeyEvent);

// glfw 루프 실행, 윈도우 close 버튼을 누르면 정상 종료
SPDLOG_INFO("Start main loop");
while(!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    context->Render();
    
    // 화면 갱신
    glfwSwapBuffers(window);
    }

context.reset();
glfwTerminate();

return 0;
}