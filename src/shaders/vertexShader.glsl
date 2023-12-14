#version 330 core

in vec3 inPosition;
in vec2 inTexcoord;
in vec3 inNormal;

out vec3 exPosition;
out vec2 exTexcoord;
out vec3 exNormal;
out vec3 exLightPos;

uniform vec3 lightPos;
uniform mat4 ModelMatrix;

uniform Camera {
   mat4 ViewMatrix;
   mat4 ProjectionMatrix;
};

void main(void)
{
	mat4 ModelView = ViewMatrix * ModelMatrix;

	vec4 MCPosition = vec4(inPosition, 1.0);
	exTexcoord = inTexcoord;
	// Eye space
	// exNormal = mat3(transpose(inverse(ModelMatrix))) * inNormal;
	// exLightPos = lightPos;
	// exPosition = vec3(ModelMatrix * MCPosition);

	exNormal = mat3(transpose(inverse(ModelView))) * inNormal;
	exLightPos = vec3(ViewMatrix * vec4(lightPos, 1));
	exPosition = vec3(ModelView * MCPosition);

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * MCPosition;
}