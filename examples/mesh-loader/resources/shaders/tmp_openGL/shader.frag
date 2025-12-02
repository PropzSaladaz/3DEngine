#version 450 core

in vec3 fragPosition;
out vec4 FragColor;
uniform vec4 inColor;

void main() {
	FragColor = fragPosition.xyzz;
}