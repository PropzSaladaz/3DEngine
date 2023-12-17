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