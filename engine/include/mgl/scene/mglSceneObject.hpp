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

	using SetShaderUniformCallback = std::function<void(ShaderProgram& shaders)>;

	/**
	 * Defines an object to be drawn within the scene it is on.
	 * Every object must have a mesh, a material, and a shader program associated.
	 * 
	 * It is then drawn following the SceneGraph tree it belongs to, applying
	 * the parent-son matrix transformations, and setting all shader
	 * information through default & custom uniforms. 
	 */
	class SceneObject : public SceneNode {
	public:
		SceneObject();
		SceneObject(std::shared_ptr<Mesh> mesh);
		SceneObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);
		SceneObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, std::shared_ptr<ShaderProgram> shaders);
		~SceneObject();

		void setMaterial(std::shared_ptr<Material> _material);
		void setShaders(std::shared_ptr<ShaderProgram> shaders);

		void setScene(Scene* scene) override;
		void setSkybox(std::shared_ptr<TextureSampler> skybox) override;
		Material& getMaterial();

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
		std::shared_ptr<ShaderProgram> shaders;
		std::shared_ptr<Mesh> mesh;
		std::shared_ptr<Material> material;
		std::vector<SetShaderUniformCallback> shaderUniformCallbacks;

	};

}

#endif