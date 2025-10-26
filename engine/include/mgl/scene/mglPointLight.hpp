#ifndef MGL_POINT_LIGHT_HPP
#define MGL_POINT_LIGHT_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <vector>

#include <mgl/mglShaders.hpp>
#include <mgl/mglTransform.hpp>
#include <mgl/scene/mglPositionalLight.hpp>
#include <mgl/scene/mglSceneObject.hpp>
#include <mgl/models/materials/mglMaterial.hpp>

namespace mgl {

	///////////////////////////////////////////////////////////////// Light
	class PointLight : public PositionalLight {
	public:
		PointLight(const glm::vec3 &position);
		PointLight(const SceneObject* position);
		PointLight(const glm::vec3 &position, const glm::vec3 &color);
		PointLight(const SceneObject* position, const glm::vec3 &color);

		void updateShaders(ShaderProgram* shader) override;
	};

	////////////////////////////////////////////////////////////////////////////////
}  // namespace mgl

#endif /* MGL_MATERIAL_HPP  */
