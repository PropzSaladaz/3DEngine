#include <mgl/scene/mglSceneObject.hpp>
#include <mgl/mglConventions.hpp>
namespace mgl {

SceneObject::SceneObject() : SceneNode() {}

SceneObject::SceneObject(Mesh* _mesh) : SceneNode(), mesh(_mesh), 
		material(nullptr), shaders(nullptr) {}
	
SceneObject::SceneObject(Mesh* mesh, Material* material) 
	: SceneObject(mesh) {
	setMaterial(material);
}

SceneObject::SceneObject(Mesh* mesh, Material* material, ShaderProgram* shaders)
	: SceneObject(mesh, material) {
	setShaders(shaders);
}
SceneObject::~SceneObject() {}


/*
	Sets default uniforms (MODEL_MATRIX) and other 
	user-defined uniforms through callbacks
*/
void SceneObject::setUniforms() {
	shaders->setUniformMatrix(MODEL_MATRIX, glm::value_ptr(AbsoluteTransform));
	for (const auto& callback : shaderUniformCallbacks) {
		callback(shaders);
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
	if (material) material->updateShaders(shaders);
	scene->updateShaders(shaders); // update with global scene info
	setUniforms();
	mesh->draw();
	shaders->unbind();
}

void SceneObject::setShaders(ShaderProgram* shaders) {
	this->shaders = shaders;
}

void SceneObject::setMaterial(Material* material) {
	this->material = material;
}

}