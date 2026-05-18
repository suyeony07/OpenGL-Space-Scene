// pro_opengl_06 src/common.h

#ifndef __COMMON_H__
#define __COMMON_H__

#include <string>
#include <optional>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <spdlog/spdlog.h>

// shader 클래스 설계 시 추가
#define CLASS_PTR(klassName)\
class klassName;\
using klassName ## UPtr = std::unique_ptr<klassName>;\
using klassName ## Ptr = std::shared_ptr<klassName>;\
using klassName ## WPtr = std::weak_ptr<klassName>;\


std::optional <std::string> LoadTextFile(const std::string& filename);

#endif // __COMMON_H__