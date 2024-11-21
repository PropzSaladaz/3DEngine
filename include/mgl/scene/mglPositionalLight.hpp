#ifndef MGL_POSITIONAL_LIGHT_HPP
#define MGL_POSITIONAL_LIGHT_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <vector>

#include <mgl/mglShaders.hpp>
#include <mgl/mglTransform.hpp>
#include <mgl/scene/mglLight.hpp>
#include <mgl/scene/mglSceneObject.hpp>
#include <mgl/models/materials/mglMaterial.hpp>

namespace mgl {

	///////////////////////////////////////////////////////////////// Light
	class PositionalLight : public Light {
	public:
		static const GLfloat DEFAULT_ATTENUATION;
		struct LightAttenuation {
			float constant;
			float linear;
			float quadratic;
		};

		PositionalLight();
		PositionalLight(const glm::vec3& position);
		PositionalLight(const SceneObject* position);
		PositionalLight(const glm::vec3& position, const glm::vec3& color);
		PositionalLight(const SceneObject* position, const glm::vec3& color);

		void setPosition(const glm::vec3& position);
		void setPosition(const SceneObject* position);
		void setAttenuation(GLuint distance);
		void setAttenuation(const LightAttenuation& attenuation);

		glm::vec3 getPosition() const;
		LightAttenuation getAttenuation() const;

		virtual void updateShaders(ShaderProgram* shader) override;

	protected:
		const SceneObject* position;

	private:
		LightAttenuation attenuation;

		// default attenuation parameters
		static inline const GLuint attenuationDistances[] = { 7, 13, 20, 32, 50, 65, 100,
			160, 200, 325, 600, 3250 };
		// distance  parameters
		static inline const std::map<GLuint, LightAttenuation> defaultAttenuations = {
			{7,	   {1.0f, 0.7f,    1.8f}},
			{13,   {1.0f, 0.35f,   0.44f}},
			{20,   {1.0f, 0.22f,   0.20f}},
			{32,   {1.0f, 0.14f,   0.07f}},
			{50,   {1.0f, 0.09f,   0.032f}},
			{65,   {1.0f, 0.07f,   0.017f}},
			{100,  {1.0f, 0.045f,  0.0075f}},
			{160,  {1.0f, 0.027f,  0.0028f}},
			{200,  {1.0f, 0.022f,  0.0019f}},
			{325,  {1.0f, 0.014f,  0.0007f}},
			{600,  {1.0f, 0.007f,  0.0002f}},
			{3250, {1.0f, 0.0014f, 0.000007f}},
		}; // Ogre3D wiki
	};

	////////////////////////////////////////////////////////////////////////////////
}  // namespace mgl

#endif /* MGL_MATERIAL_HPP  */
