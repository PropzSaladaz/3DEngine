#version 450 core

layout(std140, binding = 0) uniform Camera {
    mat4 ViewMatrix;
    mat4 ProjectionMatrix;
    vec3 CameraPosition;
};

uniform mat4 ModelMatrix;
uniform float time;

layout(location = 1) in vec3 inPosition;
layout(location = 3) in vec2 inTexcoord;

out vec2 exTexcoord;

void main() {
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(inPosition, 1.0);
    exTexcoord = inTexcoord + vec2(time, 0.0);
}
