#include <mgl/mgl.hpp>
#include <memory>

////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App {
public:
    void onRegisterGlobalResources(mgl::ResourceContext& resources) override;
    void onCreateScene(mgl::Scene& scene, mgl::ResourceContext& resources) override;

private:
    const float initialScreenRatio = 800.0f / 600.0f;
    const GLuint UBO_BP = 0;
    mgl::Scene* Scene = nullptr;
    mgl::SceneGraph* lightObj, * lightObj2;
    std::shared_ptr<mgl::FPSCamController> FPSCamera;
    GLuint cam = 0;
    mgl::PerspectiveParams perspectiveP = { 45.0f, 800.0f / 600.0f, 0.1f, 100.0f };

    // scene
    void createMeshes(mgl::MeshManager& meshManager);
    void createMaterials(mgl::MaterialManager& materialManager);
    void createShaderPrograms(mgl::ShaderManager& shaderManager);
};

///////////////////////////////////////////////////////////////////////// MESHES

void MyApp::createMeshes(mgl::MeshManager& meshManager) {
    std::shared_ptr<mgl::Mesh> mesh1 = std::make_shared<mgl::Mesh>();
    mgl::MeshData data = {
        .positions = {
            mgl::vec3(-0.5f, -0.5f, 0.0f),
            mgl::vec3(0.5f, -0.5f, 0.0f),
            mgl::vec3(0.0f, 0.5f, 0.0f)
        },
        .indices = {
            0, 1, 2
        },
    };
    mesh1->createFromData(data);

    meshManager.add("triangle", mesh1);
}

///////////////////////////////////////////////////////////////////////// SHADER

void MyApp::createShaderPrograms(mgl::ShaderManager& shaderManager) {
    mgl::ShaderBuilder simpleShaders;
    simpleShaders.addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);
    simpleShaders.addShader(GL_VERTEX_SHADER, "shaders/vert.glsl");
    simpleShaders.addShader(GL_FRAGMENT_SHADER, "shaders/frag.glsl");
    simpleShaders.addUniforms<mgl::BasicMaterial>();
    simpleShaders.addUniform(mgl::MODEL_MATRIX);
    simpleShaders.addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);

    std::shared_ptr<mgl::ShaderProgram> program = std::make_shared<mgl::ShaderProgram>(simpleShaders.build());
    shaderManager.add("simple", program);
}

///////////////////////////////////////////////////////////////////////// SCENE


void MyApp::onCreateScene(mgl::Scene& scene, mgl::ResourceContext& resources) {
    // create the camera
    auto cam = std::make_shared<mgl::PerspectiveCamera>(UBO_BP, perspectiveP);
    cam->setPosition(0.f, 0.f, 5.f);

    // target to look at the origin
    mgl::Transform target;
    target.setPosition(0.f, 0.f, 0.f);
    cam->lookAt(&target);
    scene.addCamera("mainCam", cam);

    cam->setActive();

    // build cam controller
    FPSCamera = std::make_shared<mgl::FPSCamController>(cam.get());
    FPSCamera->setMovementSpeed(2.5f);
    FPSCamera->setMouseSensitivity(0.1f);
    FPSCamera->setActive();


    std::shared_ptr<mgl::SceneObject> triangleObj = std::make_shared<mgl::SceneObject>(
        resources.meshManager.get("triangle"));

    triangleObj->setShaders(resources.shaderManager.get("simple"));
    triangleObj->setMaterial(resources.materialManager.get("basic"));
    
    std::shared_ptr<mgl::SceneGraph> triangle = std::make_shared<mgl::SceneGraph>();
    triangle->add(triangleObj);

    scene.setScenegraph(triangle);
}

void MyApp::createMaterials(mgl::MaterialManager& materialManager) {
    std::shared_ptr<mgl::Material> mat1 = std::make_shared<mgl::BasicMaterial>(mgl::vec4(1.0f, 0.5f, 0.2f, 1.0f));
    materialManager.add("basic", mat1);
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::onRegisterGlobalResources(mgl::ResourceContext& resources) {
    //glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    createMeshes(resources.meshManager);
    createMaterials(resources.materialManager);
    createShaderPrograms(resources.shaderManager);  // after mesh;
}

/////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char* argv[]) {
    mgl::Engine& engine = mgl::Engine::getInstance();
    engine.setApp(std::make_shared<MyApp>());
    engine.setOpenGL(3, 3);
    engine.setWindow(800, 600, "Mesh Loader", 0, 1);
    engine.init();
    engine.run();
    exit(EXIT_SUCCESS);
}

////////////////////////////////////////////////////////////////////////////////
