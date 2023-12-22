#include <mgl/models/materials/mglMaterial.hpp>
#include <mgl/mglShaders.hpp>
#include <mgl/mglConventions.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace mgl {
	void Material::updateShaders(ShaderProgram* shaders) {
		// set material uniforms
		setMaterialUniforms(shaders);

		if (texture) texture->updateShader(shaders); // bind texture
		if (textureSpecular) textureSpecular->updateShader(shaders); // bind texture
	}

	void Material::setTexture(TextureInfo* texture) {
		this->texture = texture;
	}

	void Material::setTextureSpecular(TextureInfo* texture) {
		this->textureSpecular = texture;
	}

}