#include <mgl/models/materials/mglMaterial.hpp>
#include <mgl/models/textures/mglTexture.hpp>
#include <mgl/mglShaders.hpp>
#include <mgl/mglConventions.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace mgl {

	void Material::updateShaders(ShaderProgram& shaders) {
		// set material uniforms
		setMaterialUniforms(shaders);
		for (auto& texture : textures) {
			texture->updateShader(shaders);
		}
	}

void Material::addTexture(std::shared_ptr<TextureSampler> texture) {
	textures.push_back(texture);
}

bool Material::setTexture(const std::string& uniform, std::shared_ptr<Texture> texture) {
	for (auto& ts : textures) {
		if (ts && ts->uniform == uniform) {
			ts->texture = std::move(texture);
			return true;
		}
	}
	return false;
}

bool Material::setTextureSampler(const std::string& uniform, std::shared_ptr<TextureSampler> sampler) {
	for (auto& ts : textures) {
		if (ts && ts->uniform == uniform) {
			ts = std::move(sampler);
			return true;
		}
	}
	textures.push_back(std::move(sampler));
	return true;
}
}
