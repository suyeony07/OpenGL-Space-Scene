// pro_opengl_06 simple.vs

#version 330 core

layout(location = 0) in vec3 aPos;

void main() {
    //정점 위치
    //gl_Position = vec4(0.0, 0.0, 0.0, 1.0);// (x,y,z,w)

    //좌표 값(오른쪽 위) 위치 변경
    gl_Position = vec4(aPos, 1.0);
}