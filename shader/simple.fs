// pro_opengl_07 shader/simple.fs

#version 330 core

out vec4 fragColor;

void main() {

    // 픽셀 색상
    fragColor = vec4(1.0, 1.0, 1.0, 1.0);

    // 픽셀 색상 바꾸기

    // fragColor = vec4(0.0, 4.0, 0.0, 1.0);

    // fragColor = vec4(0.0, 1.0, 0.0, 1.0); // 초록

    // fragColor = vec4(0.0, 0.0, 1.0, 1.0); // 파랑

    // fragColor = vec4(1.0, 1.0, 0.0, 1.0); // 노랑
}