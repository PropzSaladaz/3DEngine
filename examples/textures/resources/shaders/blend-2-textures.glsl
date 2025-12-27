#version 450 core

out vec4 FragColor;

uniform vec4 inColor;
uniform float blendFactor;
uniform sampler2D texture1;
uniform sampler2D texture2;

in vec2 exTexcoord;

void main() {
    vec4 color1 = texture(texture1, exTexcoord);
    vec4 color2 = texture(texture2, exTexcoord);
	FragColor = mix(color1, color2, blendFactor) * inColor;
}
