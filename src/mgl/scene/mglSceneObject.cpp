#include <mgl/scene/mglSceneObject.hpp>
#include <mgl/mglConventions.hpp>
namespace mgl {

	SceneObject::SceneObject(Mesh* _mesh) : SceneNode(), mesh(_mesh) {}
	
	SceneObject::SceneObject(Mesh* mesh, Material* _material) 
		: SceneObject(mesh) {
		material = _material;
	}
	SceneObject::~SceneObject() {}

	void SceneObject::draw() {
		AbsoluteTransform = (Parent == NO_PARENT) ? getTransformMatrix() :
			Parent->AbsoluteTransform * getTransformMatrix();

		material->bind();
		setUniforms();
		mesh->draw();
		material->unbind();
	}

	void SceneObject::setMaterial(Material* _material) {
		material = _material;
	}

	void SceneObject::setUniforms() {
		material->Shaders->setUniformMatrix(MODEL_MATRIX, glm::value_ptr(AbsoluteTransform));
		for (const auto& callback : shaderUniformCallbacks) {
			callback(material->Shaders);
		}
	}

	void SceneObject::setShaderUniformCallback(SetShaderUniformCallback callback) {
		shaderUniformCallbacks.push_back(callback);
	}

}