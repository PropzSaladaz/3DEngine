#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;

uniform vec4 inColor;
out vec4 FragmentColor;

vec3 lightSrc = vec3(0, 0, 1);

void main(void)
{
/**/
  // vec3 color = vec3(1.0);
  // vec3 color = (exPosition + vec3(1.0)) * 0.5;
  // vec3 color = vec3(exTexcoord, 0.0);
	float colorIntensity = max(dot(lightSrc, normalize(exNormal)) + 1.0, 1.5) / 2.0;
	FragmentColor = inColor * colorIntensity;
/** /
	vec3 N = normalize(exNormal);
	vec3 direction = vec3(1.0, 0.5, 0.25);
	float intensity = max(dot(direction, N), 0.0);
	FragmentColor = vec4(vec3(intensity), 1.0);
/**/
}