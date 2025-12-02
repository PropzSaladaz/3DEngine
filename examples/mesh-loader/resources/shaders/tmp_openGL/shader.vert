#version 450 core

layout(location = 1) in vec3 inPosition;
out vec3 fragPosition;
void main() {
    fragPosition = inPosition;
    gl_Position = vec4(inPosition.x, inPosition.y, inPosition.z, 1.0f);
}
