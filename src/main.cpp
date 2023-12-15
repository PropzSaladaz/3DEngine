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
    void windowSizeCallback(GLFWwindow* win, int width, int height) override;

private:
    const float initialScreenRatio = 800.0f / 600.0f;
    const GLuint UBO_BP = 0;
    mgl::SceneGraph* Scene = nullptr;
    mgl::MeshManager* meshes;
    mgl::ShaderManager* shaders;
    mgl::Animation* squareAnim;
    // cameras
    mgl::FPSCamController* FPSCamera;
    mgl::PerspectiveParams perspectiveP = { 45.0f, 800.0 / 600.0, 0.1, 100.0 };
    // input
    void processInput(double elapsed);
    // scene
    void createMeshes();
    void createShaderPrograms();
    void createSceneGraph();
    void createCamera();
    void drawScene();
};

///////////////////////////////////////////////////////////////////////// MESHES

void MyApp::createMeshes() {
    meshes = new mgl::MeshManager();
    meshes->setManagedItemCallback([](mgl::Mesh* mesh) {
        mesh->joinIdenticalVertices();
    });

    meshes->import("cube", "resources/models/square.obj");
    meshes->import("cube-vtn", "resources/models/statue.obj");
}

///////////////////////////////////////////////////////////////////////// SHADER

void MyApp::createShaderPrograms() {
    shaders = new mgl::ShaderManager();
    shaders->setManagedItemCallback([this](mgl::ShaderProgram* program) {
        program->addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);
        program->addAttribute(mgl::NORMAL_ATTRIBUTE, mgl::Mesh::NORMAL);
        program->addUniform(mgl::MODEL_MATRIX);
        program->addUniform(mgl::COLOR_ATTRIBUTE);
        program->addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);
        });

    mgl::ShaderProgram* squareShaders = new mgl::ShaderProgram();
    squareShaders->addShader(GL_VERTEX_SHADER, "src/shaders/vertexShader.glsl");
    squareShaders->addShader(GL_FRAGMENT_SHADER, "src/shaders/light/ambient-fs-model.glsl");
    squareShaders->addUniform("lightColor");
    squareShaders->addUniform("lightPos");

    mgl::ShaderProgram* lightShaders = new mgl::ShaderProgram();
    lightShaders->addShader(GL_VERTEX_SHADER, "src/shaders/vertexShader.glsl");
    lightShaders->addShader(GL_FRAGMENT_SHADER, "src/shaders/light/ambient-fs-light.glsl");

    shaders->add("cube", squareShaders);
    shaders->add("light", lightShaders);


}

///////////////////////////////////////////////////////////////////////// SCENE
// Colors                     R       G       B       A 
const glm::vec4 RED         (0.600f, 0.360f, 0.200f, 1.0f);
const glm::vec4 lightColor(1, 1, 1, 1);
const glm::vec4 position(0, 2, 4, 0);

void MyApp::createSceneGraph() {
    // materials
    mgl::Material* RED_M = (new mgl::Material(shaders->get("cube")))->setColor(RED);
    mgl::Transform* square_i = (new mgl::Transform())
        ->scale(0.05)
        ->rotate(-90.0f, mgl::XX)
        ->translate(0, 0, 5);
    mgl::Transform* square_f = (new mgl::Transform())
        ->scale(glm::vec3(1, 0.5, 0.5))
        ->rotate(90.0f, mgl::YY);

    mgl::Material* WHITE_M = (new mgl::Material(shaders->get("light")))->setColor(lightColor);
    mgl::Transform* light_i = (new mgl::Transform())
        ->scale(0.03)
        ->translate(0, 10, 10);

    mgl::SceneObject* lightObj = new mgl::SceneObject(meshes->get("cube"), WHITE_M);
    lightObj->setTransform(light_i);

    // scene objects
    mgl::SceneObject* squareObj = new mgl::SceneObject(meshes->get("cube-vtn"), RED_M);
    squareObj->setTransform(square_i);
    squareObj->setShaderUniformCallback([lightObj](mgl::ShaderProgram* shaders) {
        shaders->setUniformVec4f("lightColor", glm::value_ptr(lightColor));
        shaders->setUniformVec3f("lightPos", glm::value_ptr(lightObj->getPosition()));
        });


    // scene graph
    Scene = new mgl::SceneGraph(squareObj);
    Scene->add(lightObj);
    //Scene->registerCallback(
    //    []() { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }, // init
    //    []() { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }  // destroy
    //);

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
}


///////////////////////////////////////////////////////////////////////// CAMERAS

void MyApp::createCamera() {
    mgl::PerspectiveCamera* camera = new mgl::PerspectiveCamera(UBO_BP, &perspectiveP);
    FPSCamera = new mgl::FPSCamController(camera, 1, 0.1);
    FPSCamera->setPosition(0, 0, 5);
    FPSCamera->setActive();
}

/////////////////////////////////////////////////////////////////////////// DRAW

void MyApp::drawScene() {
    Scene->draw();
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow* win) {
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
