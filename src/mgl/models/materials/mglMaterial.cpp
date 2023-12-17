#include <mgl/models/materials/mglMaterial.hpp>
#include <mgl/mglShaders.hpp>
#include <mgl/mglConventions.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace mgl {
	GLuint Material::id = 0;

	Material::Material(ShaderProgram* shaders) {
		Shaders = shaders;
		shader_id = getShaderPrefix() + "_" + std::to_string(++id);
	}

	void Material::bind() {
		Shaders->bind();
		// set material uniforms
		setMaterialUniforms(Shaders);

		if (texture) texture->updateShader(Shaders); // bind texture
		if (textureSpecular) textureSpecular->updateShader(Shaders); // bind texture
	}

	void Material::unbind() {
		Shaders->unbind();
	}

	void Material::setTexture(TextureInfo* texture) {
		this->texture = texture;
	}

	void Material::setTextureSpecular(TextureInfo* texture) {
		this->textureSpecular = texture;
	}

	std::string Material::getShaderPrefix() {
		return "AbstractMaterial";
	}



}