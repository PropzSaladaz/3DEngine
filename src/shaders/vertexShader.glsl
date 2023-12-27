#version 330 core

in vec3 inPosition;
in vec2 inTexcoord;
in vec3 inNormal;

out vec3 exPosition;
out vec2 exTexcoord;
out vec3 exNormal;
out vec3 camPosition;

uniform mat4 ModelMatrix;

uniform Camera {
   mat4 ViewMatrix;
   mat4 ProjectionMatrix;
   vec3 Position;
};

void main(void)
{
	vec4 MCPosition = vec4(inPosition, 1.0);
	exTexcoord = inTexcoord;

	// World space
	exNormal = normalize(mat3(transpose(inverse(ModelMatrix))) * inNormal);
	exPosition = vec3(ModelMatrix * MCPosition);
	camPosition = Position;

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * MCPosition;
}