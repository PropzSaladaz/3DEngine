#ifndef MGL_SCENE_GRAPH_HPP
#define MGL_SCENE_GRAPH_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <mgl/mglTransform.hpp>
#include <functional>

namespace mgl {

	using InitDestroyCallback = std::function<void()>;

	class IDrawable;
	class SceneNode;
	class SceneGraph;

	class IDrawable {
	public:
		virtual void draw() = 0;
	};

	class SceneNode : public IDrawable , public Transform {
	public:
		static SceneGraph* NO_PARENT;
		SceneGraph* Parent;
		glm::mat4 AbsoluteTransform;

		glm::vec3 getAbsolutePosition();
	protected:
		SceneNode();
		~SceneNode() = default;
	};

	class SceneGraph : public SceneNode {
	public:
		SceneGraph();
		SceneGraph(SceneNode* child);
		~SceneGraph();

		void add(SceneNode* drawable);
		void remove(SceneNode* drawable);
		void draw() override;
		void registerCallback(InitDestroyCallback init, InitDestroyCallback destroy);
	private:
		std::map<GLuint, SceneNode*> children;
		InitDestroyCallback initCallback;
		InitDestroyCallback destroyCallback;

	};

}

#endif