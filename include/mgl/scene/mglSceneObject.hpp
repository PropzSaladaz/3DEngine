#ifndef MGL_SCENE_NODE_HPP
#define MGL_SCENE_NODE_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <mgl/mglTransform.hpp>
#include <mgl/scene/mglMesh.hpp>
#include <mgl/scene/mglMaterial.hpp>
#include <mgl/scene/mglSceneGraph.hpp>
#include <functional>
#include <vector>

namespace mgl {

using SetShaderUniformCallback = std::function<void(ShaderProgram* shaders)>;

class SceneObject : public SceneNode {
public:
	SceneObject(Mesh* mesh);
	SceneObject(Mesh* mesh, Material* material);
	~SceneObject();

	void draw() override;
	void setMaterial(Material* _material);
	void setUniforms();
	void setShaderUniformCallback(SetShaderUniformCallback callback);

private:
	Mesh* mesh;
	Material* material;
	std::vector<SetShaderUniformCallback> shaderUniformCallbacks;

};

}

#endif