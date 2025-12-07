#pragma once

#include <vector>
#include <string>

#include <mgl/shaders/ShaderUpdator.hpp>
#include <mgl/mglConventions.hpp>
#include <mgl/models/textures/mglTextureSampler.hpp>


#include <math/math.hpp>

namespace mgl {


const math::vec3 COLOR_WHITE  (1, 1, 1);
const math::vec3 COLOR_RED	  (1, 0, 0);
const math::vec3 COLOR_GREEN  (0, 1, 0);
const math::vec3 COLOR_BLUE	  (0, 0, 1);

class Material : public ShaderUpdator {
public:
	static inline const char MATERIAL_LIGHT_COLOR[] = "lightColor";

	void addTexture(std::shared_ptr<TextureSampler> texture);
	// Replace the texture object for an existing uniform; returns true on success
	bool setTexture(const std::string& uniform, std::shared_ptr<Texture> texture);
	// Replace or add a TextureSampler for a uniform; returns true on success
	bool setTextureSampler(const std::string& uniform, std::shared_ptr<TextureSampler> sampler);
	// Direct access for advanced mutations/toggles
	std::vector<std::shared_ptr<TextureSampler>>& getTextures() { return textures; }
	virtual Material* setColor(const math::vec3 &color) = 0; // without alpha
	virtual Material* setColor(const math::vec4 &color) = 0; // with alpha

	// override ShaderUpdator
	void updateShaders(ShaderProgram& shaders) override;

private:
	std::vector<std::shared_ptr<TextureSampler>> textures;
	virtual void setMaterialUniforms(ShaderProgram& shaders) = 0;
};

}  // namespace mgl
