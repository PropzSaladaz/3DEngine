#version 330 core

in vec3 exPosition;

uniform samplerCube Skybox;
uniform vec4 inColor; // is part of basic material, but never used

out vec4 FragColor;

void main() {
    FragColor = vec4(texture(Skybox, exPosition).rgb, 1);
    // FragColor = vec4(exPosition, 1.0);
}