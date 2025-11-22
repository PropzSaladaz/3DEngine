#include <mgl/mgl.hpp>

////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App {
public:
    void onRegisterGlobalResources(mgl::ResourceContext& resources) override;
    void onCreateScene(mgl::Scene& scene, mgl::ResourceContext& resources) override;
    void onStart() override;
    void onUpdate(double deltaTime) override;

private:
    const float initialScreenRatio = 800.0f / 600.0f;
    const GLuint UBO_BP = 0;
    mgl::Scene* Scene = nullptr;
    mgl::SceneGraph* lightObj, * lightObj2;
    // cameras
    mgl::FPSCamController* FPSCamera;
    mgl::OrbitCamController* OrbitCam;
    GLuint cam = 0;
    mgl::PerspectiveParams perspectiveP = { 45.0f, 800.0f / 600.0f, 0.1f, 100.0f };
    // input
    void processInput(double elapsed);
    void animateLights(double elapsed);

    // scene
    void createMeshes(mgl::MeshManager& meshManager);
    void createShaderPrograms(mgl::ShaderManager& shaderManager);

    void createSceneGraph();
    void createCamera();
    void drawScene();
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
    simpleShaders.addUniform(mgl::MODEL_MATRIX);
    simpleShaders.addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);
    simpleShaders.addShader(GL_VERTEX_SHADER, "shaders/tmp_openGL/shader.vert");
    simpleShaders.addShader(GL_FRAGMENT_SHADER, "shaders/tmp_openGL/shader.frag");
    simpleShaders.addUniform("triangleColor");

    std::shared_ptr<mgl::ShaderProgram> program = std::make_shared<mgl::ShaderProgram>(simpleShaders.build());
    shaderManager.add("simple", program);
    
    program->setUniform("triangleColor", mgl::vec4(1.0f, 0.5f, 0.2f, 1.0f));
}

///////////////////////////////////////////////////////////////////////// SCENE


void MyApp::onCreateScene(mgl::Scene& scene, mgl::ResourceContext& resources) {
    std::shared_ptr<mgl::SceneObject> triangleObj = std::make_shared<mgl::SceneObject>(
        resources.meshManager.get("triangle"));

    triangleObj->setShaders(resources.shaderManager.get("simple"));
    triangleObj->beforeAndAfterDraw(
        [](void) {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
        },
        [](void) {
            glDisable(GL_CULL_FACE);
        }
    );
    
    std::shared_ptr<mgl::SceneGraph> triangle = std::make_shared<mgl::SceneGraph>();
    triangle->add(triangleObj);

    scene.setScenegraph(triangle);
}

///////////////////////////////////////////////////////////////////////// INPUT

void MyApp::processInput(double elapsed) {

}

///////////////////////////////////////////////////////////////////////// CAMERAS

void MyApp::createCamera() {
    // mgl::PerspectiveCamera* camera2 = new mgl::PerspectiveCamera(UBO_BP, &perspectiveP);
    // OrbitCam = new mgl::OrbitCamController(camera2, mgl::vec3(0, 0, 0), 5.0f);
    // OrbitCam->setActive();
}

/////////////////////////////////////////////////////////////////////////// DRAW

void MyApp::animateLights(double elapsed) {
 
}


void MyApp::drawScene() {
    Scene->draw();
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow* win) {
    //glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    createMeshes();
    createTextures();
    createMaterials();
    createShaderPrograms();  // after mesh;
    createCamera();
    createSceneGraph();
}

void MyApp::windowSizeCallback(GLFWwindow* window, int winx, int winy) {
    glViewport(0, 0, winx, winy);
}

void MyApp::displayCallback(GLFWwindow* win, double elapsed) {
    //processInput(elapsed);
    //animateLights(elapsed);
    drawScene();
}

/////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char* argv[]) {
    mgl::Engine& engine = mgl::Engine::getInstance();
    engine.setApp(new MyApp());
    engine.setOpenGL(4, 6);
    engine.setWindow(800, 600, "Mesh Loader", 0, 1);
    engine.init();
    engine.run();
    exit(EXIT_SUCCESS);
}

////////////////////////////////////////////////////////////////////////////////
