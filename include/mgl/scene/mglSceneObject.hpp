#ifndef MGL_SCENE_NODE_HPP
#define MGL_SCENE_NODE_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <mgl/mglTransform.hpp>
#include <mgl/models/meshes/mglMesh.hpp>
#include <mgl/models/materials/mglMaterial.hpp>
#include <mgl/models/textures/mglTexture.hpp>
#include <mgl/scene/mglSceneGraph.hpp>
#include <functional>
#include <vector>

namespace mgl {

using SetShaderUniformCallback = std::function<void(ShaderProgram* shaders)>;

class SceneObject : public SceneNode {
public:
	SceneObject();
	SceneObject(Mesh* mesh);
	SceneObject(Mesh* mesh, Material* material);
	SceneObject(Mesh* mesh, Material* material, ShaderProgram* shaders);
	~SceneObject();

	void setMaterial(Material* _material);
	void setShaders(ShaderProgram* shaders);
	void setUniforms();
	void setShaderUniformCallback(SetShaderUniformCallback callback);

protected:
	void performDraw() override;

private:
	ShaderProgram* shaders;
	Mesh* mesh;
	Material* material;
	std::vector<SetShaderUniformCallback> shaderUniformCallbacks;

};

}

#endif