# pro_opengl_06 CMakeLists.txt

# CMAKE 이용한 외부 라이브러리 사용하기
# CMAKE 컴파일
# 주의 cmake 주석(#) 사용 // 안됨

cmake_minimum_required(VERSION 3.13...4.2)

set(PROJECT_NAME pro_opengl_06)  # set 변수 설정, 폴더 name :pro_opengl_06
set(CMAKE_CXX_STANDARD 17) # CXX : C++ 동일 STANDARD 17 VERSION

#GLFW로 윈도우를 생성하기 추가 내용
set(WINDOW_NAME "Window OpenGL Example05")
set(WINDOW_WIDTH 960)
set(WINDOW_HEIGHT 540)

project(${PROJECT_NAME})

# 추가(1) src/common.cpp src/common.h
add_executable(${PROJECT_NAME}
        src/main.cpp
        src/common.cpp src/common.h
        src/shader.cpp src/shader.h
        src/program.cpp src/program.h
        src/context.cpp src/context.h
        src/draw_objects.h
        src/draw_rocket.cpp
        src/draw_ring_planet.cpp
        src/draw_ufo.cpp
        src/draw_constellation_comet.cpp
        src/draw_stars.cpp
        ) # PROJECT_NAME의 실행 파일 만듦

include(Dependency.cmake)

# 프로젝트에 include / lib 관련 옵션 추가 // spdlog 사용
# target_include_diretories(${PROJECT_NAME} PUBLIC ${DEP_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${DEP_INCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PUBLIC ${DEP_LIB_DIR})
target_link_libraries(${PROJECT_NAME}
    spdlog
    glad
    glfw3
    "-framework Cocoa"
    "-framework OpenGL"
    "-framework IOKit"
    "-framework CoreVideo"
)

#환경변수를 macro프로젝트에 추가
target_compile_definitions(${PROJECT_NAME} PUBLIC
        WINDOW_NAME="${WINDOW_NAME}"
        WINDOW_WIDTH=${WINDOW_WIDTH}
        WINDOW_HEIGHT=${WINDOW_HEIGHT}
        )

# Dependency들이 먼저 bulid 될 수 있게 관계 설정
add_dependencies(${PROJECT_NAME} ${DEP_LIST})

# CMake가 shader 폴더를 자동 복사하게 만들기
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E
copy_directory
        ${CMAKE_SOURCE_DIR}/shader
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/shader
)