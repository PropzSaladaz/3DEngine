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

	/// <summary>
	/// Defines an object to be drawn within the scene it is on.
	/// Every object must have a mesh, a material, and a shader program associated.
	/// 
	/// It is then drawn following the SceneGraph tree it belongs to, applying
	/// the parent-son matrix transformations, and setting all shader
	/// information through default & custom uniforms. 
	/// </summary>
	class SceneObject : public SceneNode {
	public:
		SceneObject();
		SceneObject(Mesh* mesh);
		SceneObject(Mesh* mesh, Material* material);
		SceneObject(Mesh* mesh, Material* material, ShaderProgram* shaders);
		~SceneObject();

		void setMaterial(Material* _material);
		void setShaders(ShaderProgram* shaders);

		void setScene(Scene* scene) override;
		void setSkybox(TextureInfo* skybox) override;

		Material* getMaterial();

		/// <summary>
		/// Sets the default MODEL_MATRIX
		/// </summary>
		void setUniforms();

		/// <summary>
		/// Sets a callback used whenever drawing this specific SceneObject.
		/// Useful when sceneObjects have custom properties in their shaders
		/// </summary>
		/// <param name="callback"></param>
		void setShaderUniformCallback(SetShaderUniformCallback callback);

	protected:
		/// <summary>
		/// Bind the shaders associated to this object, set the shader information
		/// given by the associated material if any, 
		/// </summary>
		void performDraw() override;

	private:
		Scene* scene;
		ShaderProgram* shaders;
		Mesh* mesh;
		Material* material;
		std::vector<SetShaderUniformCallback> shaderUniformCallbacks;

	};

}

#endif