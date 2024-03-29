#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <mgl/mgl.hpp>
#include <glm/gtx/string_cast.hpp>

////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App {
public:
    void initCallback(GLFWwindow* win) override;
    void displayCallback(GLFWwindow* win, double elapsed) override;
    void windowSizeCallback(GLFWwindow* win, int width, int height) override;
    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) override;

private:
    const float initialScreenRatio = 800.0f / 600.0f;
    const GLuint UBO_BP = 0;
    mgl::ShaderProgram* Shaders = nullptr;
    mgl::SceneGraph* Scene = nullptr;
    mgl::Mesh* square;
    mgl::Animation* squareAnim;
    // cameras
    mgl::Camera* Camera;
    mgl::PerspectiveParams perspectiveP = { 45.0f, 800.0 / 600.0, 0.1, 10.0 };
    // input
    void processInput(double elapsed);
    void registerInputCallBacks();
    // scene
    void createMeshes();
    void createShaderPrograms();
    void createSceneGraph();
    void createCamera();
    void drawScene();
};

///////////////////////////////////////////////////////////////////////// MESHES

void MyApp::createMeshes() {
    square = new mgl::Mesh();
    square->joinIdenticalVertices();
    square->create("resources/models/square.obj");

}

///////////////////////////////////////////////////////////////////////// SHADER

void MyApp::createShaderPrograms() {
    Shaders = new mgl::ShaderProgram();
    Shaders->addShader(GL_VERTEX_SHADER, "src/shaders/vertexShader.glsl");
    Shaders->addShader(GL_FRAGMENT_SHADER, "src/shaders/fragShader.glsl");

    Shaders->addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);
    Shaders->addAttribute(mgl::NORMAL_ATTRIBUTE, mgl::Mesh::NORMAL);
    Shaders->addUniform(mgl::MODEL_MATRIX);
    Shaders->addUniform(mgl::COLOR_ATTRIBUTE);
    Shaders->addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);
    Shaders->create();
}

///////////////////////////////////////////////////////////////////////// SCENE
// Colors                     R       G       B       A 
const glm::vec4 RED         (1.000f, 0.360f, 0.000f, 1.0f);


void MyApp::createSceneGraph() {
    // materials
    mgl::Material* RED_M = (new mgl::Material(Shaders))->setColor(RED); 
    mgl::Transform* square_i = (new mgl::Transform())
        ->scale(glm::vec3(1, 0.5, 0.5))
        ->rotate(-90.0f - 45.0f, mgl::ZZ)
        ->translate(0, 0, -1);
    mgl::Transform* square_f = (new mgl::Transform())
        ->scale(glm::vec3(1, 0.5, 0.5))
        ->rotate(45.0f, mgl::ZZ)
        ->translate(-0.586, 0, -1);


    // scene objects
    mgl::SceneObject* squareObj = new mgl::SceneObject(square, RED_M);
    squareObj->setTransform(square_i);


    // scene graph
    Scene = new mgl::SceneGraph(squareObj);

    // animations
    squareAnim = new mgl::Animation(squareObj, square_f);
}

///////////////////////////////////////////////////////////////////////// INPUT


void MyApp::processInput(double elapsed) {
    // animation
    if (mgl::InputManager::getInstance().isKeyPressed(GLFW_KEY_RIGHT)) {
        squareAnim->step(elapsed * 8);
    }
    if (mgl::InputManager::getInstance().isKeyPressed(GLFW_KEY_LEFT)) {
        squareAnim->step(-elapsed * 8);
    }
    // movement
    float sensitivity = elapsed * 1.0f;
    glm::vec3 movementDir = { 0, 0, 0};
    if (mgl::InputManager::getInstance().isKeyPressed(GLFW_KEY_W)) {
        movementDir += sensitivity * Camera->getFrontV();
    }
    if (mgl::InputManager::getInstance().isKeyPressed(GLFW_KEY_A)) {
        movementDir += -sensitivity * Camera->getRightV();
    }
    if (mgl::InputManager::getInstance().isKeyPressed(GLFW_KEY_S)) {
        movementDir += -sensitivity * Camera->getFrontV();
    }
    if (mgl::InputManager::getInstance().isKeyPressed(GLFW_KEY_D)) {
        movementDir += sensitivity * Camera->getRightV();
    }
    Camera->translate(movementDir);

}

void MyApp::registerInputCallBacks() {
    mgl::InputManager::getInstance().registerMouseOffsetCallBack(
        [this](GLfloat xOffset, GLfloat yOffset) {
        Camera->yaw(-xOffset * 0.1);
        Camera->pitch(-yOffset * 0.1);
        }
    );
}

void MyApp::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    // do nothing
}


///////////////////////////////////////////////////////////////////////// CAMERAS

void MyApp::createCamera() {
    Camera = new mgl::Camera(UBO_BP);
    Camera->setPerspective(perspectiveP);
    Camera->setPosition(0, 0, 5);
    Camera->lookAt(new mgl::Transform(0, 0, 0));
    Camera->setActive();
}

/////////////////////////////////////////////////////////////////////////// DRAW

void MyApp::drawScene() {
    Scene->draw();
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow* win) {
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    registerInputCallBacks();
    createMeshes();
    createShaderPrograms();  // after mesh;
    createSceneGraph();
    createCamera();
}

void MyApp::windowSizeCallback(GLFWwindow* win, int winx, int winy) {
    glViewport(0, 0, winx, winy);
}

void MyApp::displayCallback(GLFWwindow* win, double elapsed) { 
    processInput(elapsed);
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
