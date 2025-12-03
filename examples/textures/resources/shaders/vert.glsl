#version 450 core

layout(std140, binding = 0) uniform Camera {
    mat4 ViewMatrix;
    mat4 ProjectionMatrix;
    vec3 CameraPosition;
};

uniform mat4 ModelMatrix;

layout(location = 1) in vec3 inPosition;

void main() {
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(inPosition, 1.0);
}
