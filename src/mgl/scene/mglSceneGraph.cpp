#include <mgl/scene/mglSceneGraph.hpp>
#include <mgl/mglConventions.hpp>
#include <mgl/models/materials/mglBasicMaterial.hpp>
#include <mgl/models/textures/mglTexture.hpp>
#include <mgl/scene/mglSceneObject.hpp>
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

Scene::Scene(MeshManager *meshes, ShaderManager *shaders, TextureManager* textures) {
	this->meshes = meshes;
	this->shaders = shaders;
	this->textures = textures;
	this->lights = new LightManager();
	this->cameras = new CameraManager();
}

void Scene::setScenegraph(SceneGraph* graph) {
	this->graph = graph;
	this->graph->setScene(this);
}

void Scene::addLight(const std::string& name, Light* light) {
	lights->add(name, light);
}

void Scene::addCamera(const std::string& name, Camera* camera) {
	cameras->add(name, camera);
}

void Scene::setSkybox(const std::string& folder, const std::string& fileType) {
	// import skybox mesh
	meshes->import(SKYBOX, "resources/models/cube-vtn.obj");
	// create skybox shader
	mgl::ShaderProgram* skyboxShaders = new mgl::ShaderProgram();
	skyboxShaders->addShader(GL_VERTEX_SHADER, "src/shaders/skyboxVS.glsl");
	skyboxShaders->addShader(GL_FRAGMENT_SHADER, "src/shaders/light/skybox.glsl");
	skyboxShaders->addUniforms<mgl::BasicMaterial>();
	skyboxShaders->addUniform(SKYBOX);
	shaders->add(SKYBOX, skyboxShaders);
	// create skybox material
	mgl::TextureCubeMap* cubemapT = new mgl::TextureCubeMap();
	cubemapT->loadCubeMap(folder, fileType);
	mgl::Sampler* cubemapS = new mgl::LinearSampler();
	cubemapS->create();
	mgl::TextureInfo* cubeTinfo = new mgl::TextureInfo(GL_TEXTURE0, 0,
		SKYBOX, cubemapT, cubemapS);
	textures->add(SKYBOX, cubeTinfo);

	mgl::Material* SKYBOX_M = new mgl::BasicMaterial();
	SKYBOX_M->addTexture(cubeTinfo);
	// create sceneNode
	mgl::SceneObject* skyboxObj = new mgl::SceneObject(
		meshes->get(SKYBOX),
		SKYBOX_M,
		shaders->get(SKYBOX));

	skyboxObj->beforeAndAfterDraw(
		[]() { // before
			glDepthFunc(GL_LEQUAL); // Skybox z value will be 1
			glCullFace(GL_FRONT);
		},
		[]() { // after
			glCullFace(GL_BACK);
			glDepthFunc(GL_LESS);
		});

	this->skybox = skyboxObj;
	graph->setSkybox(cubeTinfo);
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
	// usually draw skybox at end for performance
	// but we draw it first because we have transparent objects
	// and we need to draw the background first to see the transparency
	if (skybox) skybox->draw();
	if (graph)  graph ->draw();
}

void Scene::updateShaders(ShaderProgram* shaders) {
	// check if lights are defined for current shader
	if (shaders->isUniform(Light::LIGHT_UNIFORM(0, Light::LIGHT_POSITION_PROP))) {
		lights->updateShaders(shaders);
	}
}

//////////////////////////////////////////////////////////////// SceneNode

SceneGraph* SceneNode::NO_PARENT = nullptr;

SceneNode::SceneNode() : IDrawable(), Transform(), 
	Parent(NO_PARENT), AbsoluteTransform(I) {}

glm::vec3 SceneNode::getAbsolutePosition() const {
	return glm::vec3(AbsoluteTransform * glm::vec4(getPosition(), 1.0f));
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
	for (const auto& node : children) {
		node.second->setScene(scene);
	}
}

void SceneGraph::setSkybox(TextureInfo* skybox) {
	for (const auto& node : children) {
		node.second->setSkybox(skybox);
	}
}

}