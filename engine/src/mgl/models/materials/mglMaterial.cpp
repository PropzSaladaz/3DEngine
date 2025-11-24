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

	void Material::addTexture(std::shared_ptr<TextureInfo> texture) {
		textures.push_back(texture);
	}
}