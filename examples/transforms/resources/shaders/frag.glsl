#version 450 core

out vec4 FragColor;

uniform vec4 inColor;
uniform sampler2D texture1;

in vec2 exTexcoord;

void main() {
	FragColor = texture(texture1, exTexcoord) * inColor;
}
