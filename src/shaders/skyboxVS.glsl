#version 330 core

in vec3 inPosition;
in vec2 inTexcoord;
in vec3 inNormal;

out vec3 exPosition;
out vec2 exTexcoord;
out vec3 exNormal;

uniform mat4 ModelMatrix;

uniform Camera {
   mat4 ViewMatrix;
   mat4 ProjectionMatrix;
};

void main(void)
{
	vec4 MCPosition = vec4(inPosition, 1.0);
    mat4 ViewWithoutTranslation = mat4(mat3(ViewMatrix));
	exPosition = inPosition;

	gl_Position = (ProjectionMatrix * ViewWithoutTranslation * ModelMatrix * MCPosition).xyww;
}