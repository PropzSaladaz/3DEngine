#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <mgl/mgl.hpp>
#include <glm/gtx/string_cast.hpp>
#include <logger.hpp>

////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App {
public:
    void initCallback(GLFWwindow* win) override;
    void displayCallback(GLFWwindow* win, double elapsed) override;
    void windowSizeCallback(GLFWwindow* window, int width, int height) override;

private:
    const float initialScreenRatio = 800.0f / 600.0f;
    const GLuint UBO_BP = 0;
    mgl::Scene* Scene = nullptr;
    mgl::SceneGraph* lightObj, * lightObj2;
    mgl::MeshManager* meshes;
    mgl::ShaderManager* shaders;
    mgl::TextureManager* textures;
    mgl::MaterialManager* materials;
    mgl::Animation* Animation;
    // cameras
    mgl::FPSCamController* FPSCamera;
    mgl::OrbitCamController* OrbitCam;
    GLuint cam = 0;
    mgl::PerspectiveParams perspectiveP = { 45.0f, 800.0f / 600.0f, 0.1f, 100.0f };
    // input
    void processInput(double elapsed);
    void animateLights(double elapsed);
    // scene
    void createMeshes();
    void createShaderPrograms();
    void createTextures();
    void createMaterials();
    void createSceneGraph();
    void createCamera();
    void drawScene();
};

///////////////////////////////////////////////////////////////////////// MESHES

void MyApp::createMeshes() {
    meshes = new mgl::MeshManager();
    meshes->meshConfigCallback([](mgl::Mesh* mesh) {
        mesh->joinIdenticalVertices();
        });
}

///////////////////////////////////////////////////////////////////////// TEXTURES

void MyApp::createTextures() {
    textures = new mgl::TextureManager();

}

///////////////////////////////////////////////////////////////////////// MATERIALS

void MyApp::createMaterials() {
    materials = new mgl::MaterialManager();
}

///////////////////////////////////////////////////////////////////////// SHADER

void MyApp::createShaderPrograms() {
    shaders = new mgl::ShaderManager();

    mgl::ShaderProgram* chunkShaders = new mgl::ShaderProgram();
    chunkShaders->addShader(GL_VERTEX_SHADER, "shaders/test/vertex.glsl");
    chunkShaders->addShader(GL_FRAGMENT_SHADER, "shaders/test/frag.glsl");

}

///////////////////////////////////////////////////////////////////////// SCENE


void MyApp::createSceneGraph() {

}

///////////////////////////////////////////////////////////////////////// INPUT

void MyApp::processInput(double elapsed) {
    // animation
    if (mgl::InputManager::getInstance().isKeyPressed(GLFW_KEY_RIGHT)) {
        Animation->step(elapsed * 16);
    }
    if (mgl::InputManager::getInstance().isKeyPressed(GLFW_KEY_LEFT)) {
        Animation->step(-elapsed * 16);
    }
}

///////////////////////////////////////////////////////////////////////// CAMERAS

void MyApp::createCamera() {
    mgl::PerspectiveCamera* camera2 = new mgl::PerspectiveCamera(UBO_BP, &perspectiveP);
    OrbitCam = new mgl::OrbitCamController(camera2, glm::vec3(0, 0, 0), 5.0f);
    OrbitCam->setActive();
}

/////////////////////////////////////////////////////////////////////////// DRAW

void MyApp::animateLights(double elapsed) {
    lightObj->rotate(elapsed * 15, mgl::YY);
    lightObj2->rotate(elapsed * 15, mgl::YY);
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
    //drawScene();
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
