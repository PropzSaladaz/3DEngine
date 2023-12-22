#ifndef MGL_DIRECTIONAL_LIGHT_HPP
#define MGL_DIRECTIONAL_LIGHT_HPP

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
	class PointLight : public Light {
	public:
		PointLight(const glm::vec3 &position);
		PointLight(const SceneObject* position);
		PointLight(const glm::vec3 &position, const glm::vec3 &color);
		PointLight(const SceneObject* position, const glm::vec3 &color);

		void setColor(const glm::vec3 &color) override;
		void setPosition(const glm::vec3& position);
		void setAmbient(const glm::vec3 &ambient);
		void setDiffuse(const glm::vec3 &diffuse);
		void setSpecular(const glm::vec3 &specular);

		glm::vec3 getPosition();
		glm::vec3 getAmbient();
		glm::vec3 getDiffuse();
		glm::vec3 getSpecular();

		void updateShaders(ShaderProgram* shader) override;

	private:
		glm::vec3 ambientColor;
		glm::vec3 diffuseColor;
		glm::vec3 specularColor;
		const SceneObject* position;
	};

	////////////////////////////////////////////////////////////////////////////////
}  // namespace mgl

#endif /* MGL_MATERIAL_HPP  */
