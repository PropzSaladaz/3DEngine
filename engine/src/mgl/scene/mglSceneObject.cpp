#include <mgl/scene/mglSceneObject.hpp>
#include <mgl/mglConventions.hpp>
namespace mgl {

SceneObject::SceneObject() : SceneNode() {}

SceneObject::SceneObject(std::shared_ptr<Mesh> _mesh) : SceneNode(), mesh(_mesh), 
		material(nullptr), shaders(nullptr), scene(nullptr) {}
	
SceneObject::SceneObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material) 
	: SceneObject(mesh) {
	setMaterial(material);
}

SceneObject::SceneObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, std::shared_ptr<ShaderProgram> shaders)
	: SceneObject(mesh, material) {
	setShaders(shaders);
}

SceneObject::~SceneObject() {}


/*
	Sets default uniforms (MODEL_MATRIX) and other 
	user-defined uniforms through callbacks
*/
void SceneObject::setUniforms() {
	// TODO - allows no use of camera model, but is slower
	if (shaders->isUniform(MODEL_MATRIX)) shaders->setUniform(MODEL_MATRIX, AbsoluteTransform);
	
	for (const auto& callback : shaderUniformCallbacks) {
		callback(*shaders);
	}
}

void SceneObject::setShaderUniformCallback(SetShaderUniformCallback callback) {
	shaderUniformCallbacks.push_back(callback);
}

/*
	Update associated shaders with current absolute model matrix,
	material uniforms, and scene global information such as lights
*/
void SceneObject::performDraw() {
	AbsoluteTransform = (Parent == NO_PARENT) ? getTransformMatrix() :
		Parent->AbsoluteTransform * getTransformMatrix();

	shaders->bind();
	if (material) material->updateShaders(*shaders);
	if (scene) scene->updateShaders(*shaders); // update with global scene info
	setUniforms();
	mesh->draw();
	shaders->unbind();
}

void SceneObject::setShaders(std::shared_ptr<ShaderProgram> shaders) {
	this->shaders = shaders;
}

void SceneObject::setMaterial(std::shared_ptr<Material> material) {
	this->material = material;
}

void SceneObject::setScene(Scene* scene) {
	this->scene = scene;
}

void SceneObject::setSkybox(std::shared_ptr<TextureSampler> skybox) {
	// only add skybox to shaders that are expecting it
	if (material && shaders->isUniform(skybox->uniform)) {
		material->addTexture(skybox);
	}
}

Material& SceneObject::getMaterial() {
	return *material;
}

}