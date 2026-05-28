#version 330 core
uniform float uAlpha;
uniform vec3 uColor;
out vec4 fragColor;
void main() {
    fragColor = vec4(uColor, uAlpha);
}