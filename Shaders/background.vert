#version 400 core
out vec2 screenPosition;
void main() {
    vec2 p = vec2((gl_VertexID << 1) & 2, gl_VertexID & 2);
    screenPosition = p * 2.0 - 1.0;
    gl_Position = vec4(screenPosition, 0.0, 1.0);
}
