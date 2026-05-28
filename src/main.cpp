// pro_opengl_06 src/main.cpp

#include "context.h"

#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// GLFW CALLBACKS
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

            // context로 키 전달
            auto context = (Context*)glfwGetWindowUserPointer(window);
            if (context) {
                context->ProcessInput(key, action);
            }
        }


int main(int argc, const char** argv){
    SPDLOG_INFO("start program");

    SPDLOG_INFO("Initialize glfw");
    if(!glfwInit()) {
        const char* description;
        glfwGetError(&description);
        SPDLOG_ERROR("failed to Initialize glfw: {}", description);
   return -1;
    }

glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 

SPDLOG_INFO("Create glfw window");
auto window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
if(!window) {
    SPDLOG_ERROR("failed to Initialize glfw window");
    glfwTerminate();
    return -1;
}

glfwMakeContextCurrent(window);

if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        SPDLOG_ERROR("failed to Initialize glad");
        glfwTerminate();
        return -1;
}

auto glVersion = glGetString(GL_VERSION);
SPDLOG_INFO("OpenGL context version: {}", (const char*)glVersion);

auto context = Context::Create();
if (!context) {
    SPDLOG_ERROR("failed to create context");
    glfwTerminate();
    return -1;
}

// context를 window에 연결
glfwSetWindowUserPointer(window, context.get());

// GLFW CALLBACKS
OnFramebufferSizeChange(window, WINDOW_WIDTH, WINDOW_HEIGHT);
glfwSetFramebufferSizeCallback(window, OnFramebufferSizeChange);
glfwSetKeyCallback(window, OnKeyEvent);

SPDLOG_INFO("Start main loop");
while(!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    context->Render();
    glfwSwapBuffers(window);
    }

context.reset();
glfwTerminate();

return 0;
}