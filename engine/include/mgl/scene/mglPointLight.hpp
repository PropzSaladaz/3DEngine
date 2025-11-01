#pragma once

#include <math/math.hpp>

#include <mgl/mglShaders.hpp>
#include <mgl/mglTransform.hpp>
#include <mgl/scene/mglPositionalLight.hpp>
#include <mgl/scene/mglSceneObject.hpp>
#include <mgl/models/materials/mglMaterial.hpp>

namespace mgl {

	class PointLight : public PositionalLight {
	public:
		PointLight(const math::vec3 &position);
		PointLight(const SceneObject* position);
		PointLight(const math::vec3 &position, const math::vec3 &color);
		PointLight(const SceneObject* position, const math::vec3 &color);

		void updateShaders(ShaderProgram* shader) override;
	};

}  // namespace mgl
