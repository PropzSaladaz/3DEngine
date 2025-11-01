#pragma once

#include <vector>

#include <mgl/mglShaders.hpp>
#include <mgl/mglConventions.hpp>
#include <mgl/models/textures/mglTexture.hpp>

#include <math/math.hpp>

namespace mgl {


const math::vec3 COLOR_WHITE	(1, 1, 1);
const math::vec3 COLOR_RED	(1, 0, 0);
const math::vec3 COLOR_GREEN	(0, 1, 0);
const math::vec3 COLOR_BLUE	(0, 0, 1);

/////////////////////////////////////////////////////////////////////////// Material
class Material : public ShaderUpdator {
public:
	static inline const char MATERIAL_LIGHT_COLOR[] = "lightColor";

	void addTexture(TextureInfo* texture);
	virtual Material* setColor(const math::vec3 &color) = 0; // without alpha
	virtual Material* setColor(const math::vec4 &color) = 0; // with alpha
	void updateShaders(ShaderProgram* shaders) override;

private:
	std::vector<TextureInfo*> textures;
	virtual void setMaterialUniforms(ShaderProgram* shaders) = 0;
};

////////////////////////////////////////////////////////////////////////////////
}  // namespace mgl

