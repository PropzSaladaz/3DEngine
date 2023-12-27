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

	class Scene : public IDrawable, public ShaderUpdator {
	public:
		Scene(MeshManager* meshes, ShaderManager* shaders, TextureManager* textures);
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

	/*
		Represents a generic abstract node in the scene graph
		Has a parent node and an absolute transform
	*/
	class SceneNode : public IDrawable , public Transform {
	public:
		SceneGraph* Parent;
		glm::mat4 AbsoluteTransform;

		glm::vec3 getAbsolutePosition() const;
		virtual void setScene(Scene* scene) = 0;
		virtual void setSkybox(TextureInfo* skybox) = 0;

	protected:
		static SceneGraph* NO_PARENT;
		SceneNode();
		~SceneNode() = default;
	};


	/*
		Represents a scene graph that may have children.
		It can also be the child of another scene graph.
	*/
	class SceneGraph : public SceneNode {
	public:
		SceneGraph();
		SceneGraph(SceneNode* child);
		~SceneGraph();

		void add(SceneNode* drawable);
		void remove(SceneNode* drawable);

		void setScene(Scene* scene) override;
		void setSkybox(TextureInfo* skybox) override;

	protected:
		void performDraw() override;

	private:
		std::map<GLuint, SceneNode*> children;
	};

}

#endif