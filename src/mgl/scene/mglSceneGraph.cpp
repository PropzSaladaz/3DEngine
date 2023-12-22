#include <mgl/scene/mglSceneGraph.hpp>
#include <logger.hpp>

namespace mgl {

//////////////////////////////////////////////////////////////// IDrawable

void IDrawable::draw() {
	for (auto& callback : beforeDrawCallbacks) {
		callback();
	}
	performDraw();
	for (auto& callback : afterDrawCallbacks) {
		callback();
	}
}

void IDrawable::beforeDraw(IDrawableVoidCallback before) {
	beforeDrawCallbacks.push_back(before);
}

void IDrawable::afterDraw(IDrawableVoidCallback after) {
	afterDrawCallbacks.push_back(after);
}

void IDrawable::beforeAndAfterDraw(IDrawableVoidCallback before, IDrawableVoidCallback after) {
	beforeDraw(before);
	afterDraw(after);
}

//////////////////////////////////////////////////////////////// Scene

Scene::Scene(SceneGraph* graph) {
	this->graph = graph;
	this->graph->setScene(this);
	this->lights = new LightManager();
	this->cameras = new CameraManager();
}

void Scene::addLight(const std::string& name, Light* light) {
	lights->add(name, light);
}

void Scene::addCamera(const std::string& name, Camera* camera) {
	cameras->add(name, camera);
}

void Scene::assignLightToCamera(const std::string& light, const std::string& camera) {
	Light* lightObj = lights->get(light);
	Camera* cam = cameras->get(camera);

	#ifdef DEBUG
		if (!lightObj) {
			util::Logger::LogError("No light with name " + light + " exists in current scene");
			exit(EXIT_FAILURE);
		}
		else if (!cam) {
			util::Logger::LogError("No camera with name " + camera + " exists in current scene");
			exit(EXIT_FAILURE);
		}
	#endif

	lightObj->assignCamera(cam);
}

void Scene::performDraw() {
	graph->draw();
}

void Scene::updateShaders(ShaderProgram* shaders) {
	// check light is defined for current shader
	if (shaders->isUniform(Light::LIGHT_POSITION)) {
		lights->updateShaders(shaders);
	}
}

//////////////////////////////////////////////////////////////// SceneNode

SceneGraph* SceneNode::NO_PARENT = nullptr;

SceneNode::SceneNode() : IDrawable(), Transform(), 
	Parent(NO_PARENT), AbsoluteTransform(I), scene(nullptr) {}

glm::vec3 SceneNode::getAbsolutePosition() const {
	return glm::vec3(AbsoluteTransform * glm::vec4(getPosition(), 1.0f));
}

void SceneNode::setScene(Scene* scene) {
	this->scene = scene;
}

////////////////////////////////////////////////////////////////// SceneGraph

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

void SceneGraph::performDraw() {
	AbsoluteTransform = Parent == NO_PARENT ? getTransformMatrix() :
				  Parent->AbsoluteTransform * getTransformMatrix();

	for (const auto& node : children) {
		node.second->draw();
	}
}

void SceneGraph::setScene(Scene* scene) {
	this->scene = scene;
	for (const auto& node : children) {
		node.second->setScene(scene);
	}
}

}