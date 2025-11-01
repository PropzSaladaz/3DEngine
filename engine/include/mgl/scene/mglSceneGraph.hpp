#ifndef MGL_SCENE_GRAPH_HPP
#define MGL_SCENE_GRAPH_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <mgl/mglTransform.hpp>
#include <mgl/scene/mglDrawable.hpp>
#include <mgl/scene/mglLight.hpp>
#include <mgl/scene/mglLightManager.hpp>
#include <mgl/camera/mglCameraManager.hpp>
#include <mgl/mglShaderManager.hpp>
#include <mgl/models/meshes/mglMeshManager.hpp>
#include <mgl/models/textures/mglTextureManager.hpp>
#include <string>

namespace mgl {

	class Scene;
	class SceneNode;
	class SceneGraph;

	//////////////////////////////////////////////////////////////// Scene

	/**
	 * Represents the highest level of object hierarchy. A scene is responsible for
	 * all underlying object's relationships, and is responsible for parsing all objects
	 * drawing them along with their parent-son relationships.
	 *
	 * A scene stores its set of meshes through a mesh manager
	 * Scenes also manage shader programs used, lights, textures, cameras, and the skybox.
	 *
	 */
	class Scene : public IDrawable, public ShaderUpdator {
	public:
		Scene(MeshManager* meshes, ShaderManager* shaders, TextureManager* textures);
		~Scene();
		void addLight(const std::string &name, Light* light);
		void addCamera(const std::string& name, Camera* camera);
		void setScenegraph(SceneGraph* graph);
		void setSkybox(const std::string& folder, const std::string& fileType);
		void assignLightToCamera(const std::string& light, const std::string& camera);
		void updateShaders(ShaderProgram* shaders) override;
	protected:
		void performDraw() override;
	private:
		SceneGraph* graph;
		LightManager* lights;
		ShaderManager* shaders;
		MeshManager* meshes;
		TextureManager* textures;
		CameraManager* cameras;
		SceneNode* skybox;

	};

	//////////////////////////////////////////////////////////////// Scene Node

	/** Represents an abstract node in the SceneGraph.
	 * Stores a reference to its parent.
	 */
	class SceneNode : public IDrawable , public Transform {
	public:
		SceneGraph* Parent;
		math::mat4 AbsoluteTransform;

		math::vec3 getAbsolutePosition() const;
		virtual void setScene(Scene* scene) = 0;
		virtual void setSkybox(TextureInfo* skybox) = 0;

	protected:
		static SceneGraph* NO_PARENT;
		SceneNode();
		~SceneNode() = default;
	};

	//////////////////////////////////////////////////////////////// Scene Graph

	/** Represents a branchable scene node, allowing it to have several SceneNodes as children.
	 * Children positions will be relative this SceneGraph.
	 *
	 * Should only be used for objects that are expected to be modified during runtime.
	 *
	 * When a SceneGraph is drawn, it starts at the highest parent, traversing through
	 * all its children in a Depth First Search style
	 */
	class SceneGraph : public SceneNode {
	public:
		SceneGraph();
		SceneGraph(SceneNode* child);
		~SceneGraph();

		/// <summary>
		/// Adds a child
		/// </summary>
		/// <param name="child">reference of the child to be added</param>
		void add(SceneNode* child);

		/// <summary>
		/// Removes a child
		/// </summary>
		/// <param name="child">reference of the child to be removed</param>
		void remove(SceneNode* child);

		void setScene(Scene* scene) override;
		void setSkybox(TextureInfo* skybox) override;

	protected:
		/// <summary>
		/// Updates its absolute transform based on its parent's
		/// and calls draw on all its children.
		/// </summary>
		void performDraw() override;

	private:
		std::map<ui32, SceneNode*> children;
	};

}

#endif