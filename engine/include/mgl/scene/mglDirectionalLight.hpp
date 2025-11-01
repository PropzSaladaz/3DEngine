#ifndef MGL_DIRECTIONAL_LIGHT_HPP
#define MGL_DIRECTIONAL_LIGHT_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math/math.hpp>
#include <iostream>
#include <string>
#include <vector>

#include <mgl/mglShaders.hpp>
#include <mgl/mglTransform.hpp>
#include <mgl/scene/mglPositionalLight.hpp>
#include <mgl/scene/mglSceneObject.hpp>
#include <mgl/models/materials/mglMaterial.hpp>

namespace mgl {
	/*
		Represents directional light, simulating a source of light
		at an infinite distance, with no position, and only a direction
		vector.
	*/
	class DirectionalLight : public Light {
	public:
		DirectionalLight(const math::vec3& direction);
		DirectionalLight(const math::vec3& direction, const math::vec3& color);

		void setDirection(const math::vec3 &direction);
		math::vec3 getDirection() const;

		void updateShaders(ShaderProgram* shader) override;

	private:
		math::vec3 direction;
	};

	////////////////////////////////////////////////////////////////////////////////
}  // namespace mgl

#endif /* MGL_MATERIAL_HPP  */
