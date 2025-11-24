#pragma once

#include <mgl/scene/mglPositionalLight.hpp>

namespace mgl {

class SpotLight : public PositionalLight {

public:
	static const f32 DEFAULT_INNER_COS_CUTOFF;
	static const f32 DEFAULT_OUTER_COS_CUTOFF;
	static const f32 DEFAULT_EPSILON;

	SpotLight();
	SpotLight(const math::vec3 &position);
	SpotLight(const std::shared_ptr<SceneObject> position);
	SpotLight(const math::vec3 &position, const math::vec3 &color);
	SpotLight(const std::shared_ptr<SceneObject> position, const math::vec3 &color);
	SpotLight(const math::vec3 &position, const math::vec3 &color, const math::vec3 &target);
	SpotLight(const math::vec3 &position, const math::vec3 &color, const std::shared_ptr<SceneObject> target);
	SpotLight(const std::shared_ptr<SceneObject> position, const math::vec3 &color, const math::vec3 & target);
	SpotLight(const std::shared_ptr<SceneObject> position, const math::vec3 &color, const std::shared_ptr<SceneObject> target);

	void setTarget(const math::vec3 &target);
	void setTarget(const std::shared_ptr<SceneObject> target);
	void setInnerCutoffAngle(f32 angleDegrees);
	void setOuterCutoffAngle(f32 angleDegrees);

	math::vec3 getDirection();

	void updateShaders(ShaderProgram& shader) override;

private:
	std::shared_ptr<SceneObject> target = nullptr;
	f32 spotOuterCosCutoff = DEFAULT_OUTER_COS_CUTOFF;
	f32 spotInnerCosCutoff = DEFAULT_INNER_COS_CUTOFF;
	f32 epsilon = DEFAULT_EPSILON;
};

}