// pro_opengl_06 src/common.cpp

#include "common.h"
#include <fstream>
#include <sstream>

std::optional<std::string> LoadTextFile(const std::string& filename){
    std::ifstream fin(filename, std::ios::binary);

    if (!fin){
        SPDLOG_ERROR("failed to open file : {}", filename);
        return std::nullopt;
    }

    std::stringstream text;
    text << fin.rdbuf();

    return text.str();
}