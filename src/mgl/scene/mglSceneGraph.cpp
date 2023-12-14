#include <mgl/scene/mglSceneGraph.hpp>

namespace mgl {

// SceneNode
SceneGraph* SceneNode::NO_PARENT = nullptr;

SceneNode::SceneNode() : IDrawable(), Transform(), 
	Parent(NO_PARENT), AbsoluteTransform(I) {}



// SceneGraph
SceneGraph::~SceneGraph(void) {
	children.clear();
}

SceneGraph::SceneGraph() : SceneNode() {}

SceneGraph::SceneGraph(SceneNode* child) : SceneGraph() {
	add(child);
}

void SceneGraph::add(SceneNode* child) {
	children.insert(std::make_pair(child->getId(), child));
	child->Parent = this;
}
void SceneGraph::remove(SceneNode* child) {
	child->Parent = NO_PARENT;
	children.erase(child->getId());
}

void SceneGraph::draw() {

	if (initCallback) {
		initCallback();
	}

	AbsoluteTransform = Parent == NO_PARENT ? getTransformMatrix() :
				  Parent->AbsoluteTransform * getTransformMatrix();
	for (const auto& node : children) {
		node.second->draw();
	}

	if (destroyCallback) {
		destroyCallback();
	}
}

void SceneGraph::registerCallback(InitDestroyCallback init, InitDestroyCallback destroy) {
	initCallback = init;
	destroyCallback = destroy;
}

}