#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <mgl/mgl.hpp>

////////////////////////////////////////////////////////////////////////// ORBIT CAMERA

class OrbitCamera : public mgl::Camera {
public:
    glm::quat rotation = glm::angleAxis(0.0f, mgl::YY);
    GLfloat radius = 1.0f;
    Transform* center;
    OrbitCamera(const glm::vec3& centerP, GLfloat _radius, GLuint bindingPoint)
        : mgl::Camera(bindingPoint) {
        radius = _radius;
        center = new Transform(centerP);
        setPosition(0, 0, radius);
        trackTarget(center);
    }

    void rotate(float degrees, const glm::vec3& axis) {
        glm::quat rot = glm::angleAxis(glm::radians(degrees), axis);
        rotation = glm::normalize(rot * rotation);
        glm::quat newPosQ = rot * glm::quat(0, positionV) * glm::inverse(rot);
        setPosition(glm::vec3(newPosQ.x, newPosQ.y, newPosQ.z));
    }

    void zoom(float step) {
        radius -= step;
        if (radius < 1.0f) radius = 1.0f;
        else mgl::Camera::zoom(step);
    }
};

////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App {
public:
    void initCallback(GLFWwindow* win) override;
    void displayCallback(GLFWwindow* win, double elapsed) override;
    void windowSizeCallback(GLFWwindow* win, int width, int height) override;
    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) override;

private:
    const float initialScreenRatio = 800.0f / 600.0f;
    const enum MESHES { SQUARE, PARALLELOGRAM, TRIANGLE, N_MESHES};
    const enum ANIMATION { SQUARE_GREEN, PARALLELOGRAM_ORANGE, TRIANGLE_RED, TRIANGLE_PINK, 
        TRIANGLE_PURPLE, TRIANGLE_BLUE, TRIANGLE_GREEN, SCENE, N_ANIMATIONS};
    const GLuint UBO_BP = 0;
    mgl::ShaderProgram* Shaders = nullptr;
    mgl::SceneGraph* Scene = nullptr;
    mgl::Mesh* MESHES[N_MESHES];
    mgl::Animation* ANIMATION[N_ANIMATIONS];
    // cameras
    OrbitCamera* OrbitCams[2];
    mgl::PerspectiveParams perspectiveP = { 60.0f, initialScreenRatio , 0.1f, 20.0f };
    mgl::OrthoParams orthoP             = { -5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 20.0f };
    bool OrbitCamPerspective[2] = { true, false };
    GLuint currCam = 0;
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
    MESHES[SQUARE] = new mgl::Mesh();
    MESHES[SQUARE]->joinIdenticalVertices();
    MESHES[SQUARE]->create("resources/models/square.obj");

    MESHES[PARALLELOGRAM] = new mgl::Mesh();
    MESHES[PARALLELOGRAM]->joinIdenticalVertices();
    MESHES[PARALLELOGRAM]->create("resources/models/parallelogram.obj");

    MESHES[TRIANGLE] = new mgl::Mesh();
    MESHES[TRIANGLE]->joinIdenticalVertices();
    MESHES[TRIANGLE]->create("resources/models/triangle.obj");

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
const glm::vec4 GREEN       (0.000f, 0.630f, 0.060f, 1.0f);
const glm::vec4 PINK        (0.824f, 0.305f, 0.450f, 1.0f);
const glm::vec4 BLUE        (0.043f, 0.447f, 0.925f, 1.0f);
const glm::vec4 PURPLE      (0.455f, 0.129f, 0.655f, 1.0f);
const glm::vec4 OCEAN_GREEN (0.247f, 0.659f, 0.682f, 1.0f);
const glm::vec4 ORANGE      (1.000f, 0.596f, 0.122f, 1.0f);

void MyApp::createSceneGraph() {
    // materials
    mgl::Material* RED_M = (new mgl::Material(Shaders))->setColor(RED); 
    mgl::Material* GREEN_M = (new mgl::Material(Shaders))->setColor(GREEN); 
    mgl::Material* PINK_M = (new mgl::Material(Shaders))->setColor(PINK); 
    mgl::Material* BLUE_M = (new mgl::Material(Shaders))->setColor(BLUE); 
    mgl::Material* PURPLE_M = (new mgl::Material(Shaders))->setColor(PURPLE); 
    mgl::Material* OCEAN_GREEN_M = (new mgl::Material(Shaders))->setColor(OCEAN_GREEN); 
    mgl::Material* ORANGE_M = (new mgl::Material(Shaders))->setColor(ORANGE);

    // Initial transforms
    mgl::Transform* p_orange_i = (new mgl::Transform())
        ->scale(glm::vec3(1, 0.5, 0.5)) // maintain width
        ->rotate(90.0f, mgl::YY)
        ->translate(-1.414f, 0, 0);
    mgl::Transform* s_green_i = (new mgl::Transform())
        ->scale(glm::vec3(1, 0.5, 0.5))
        ->rotate(-90.0f - 45.0f, mgl::ZZ)
        ->translate(0, 0, -1);
    mgl::Transform* t_pink_i =  (new mgl::Transform())
        ->rotate(90.0f, mgl::YY)
        ->rotate(45.0f, mgl::ZZ);
    mgl::Transform* t_blue_i = (new mgl::Transform())
        ->rotate(90.0f, mgl::YY)
        ->rotate(-45.0f, mgl::ZZ);
    mgl::Transform* t_purple_i = (new mgl::Transform())
        ->scale(glm::vec3(1, 0.707, 0.707))
        ->rotate(90.0f, mgl::YY)
        ->translate(-1.414f, -1.414f, 0);
    mgl::Transform* t_ocean_green_i = (new mgl::Transform())
        ->scale(glm::vec3(1, 0.5, 0.5))
        ->rotate(90.0f, mgl::YY)
        ->rotate(90.0f + 45.0f, mgl::ZZ);
    mgl::Transform* t_red_i = (new mgl::Transform())
        ->scale(glm::vec3(1, 0.5, 0.5))
        ->rotate(90.0f, mgl::YY)
        ->rotate(-90.0f - 45.0f, mgl::ZZ)
        ->translate(0.707, -0.707, 0);

    // Final transforms
    mgl::Transform* scene_f = (new mgl::Transform())
        ->rotate(90.0f, mgl::YY)
        ->translate(-2.0f, 0, 0);
    mgl::Transform* p_orange_f = (new mgl::Transform())
        ->scale(glm::vec3(1, 0.5, 0.5))
        ->rotate(90.0f, mgl::YY)
        ->translate(0, 2, 0);
    mgl::Transform* s_green_f = (new mgl::Transform())
        ->scale(glm::vec3(1, 0.5, 0.5))
        ->rotate(45.0f, mgl::ZZ)
        ->translate(-0.586, 0, -1);
    mgl::Transform* t_pink_f = (new mgl::Transform())
        ->scale(glm::vec3(0.99, 1, 1))
        ->rotate(90.0f, mgl::YY)
        ->rotate(90.0f, mgl::ZZ)
        ->translate(1.414f, 0, 0.005);
    mgl::Transform* t_blue_f = (new mgl::Transform())
        ->scale(glm::vec3(1.02, 1, 1))
        ->rotate(90.0f, mgl::YY)
        ->translate(0, 0, 0.01);
    mgl::Transform* t_purple_f = (new mgl::Transform())
        ->scale(glm::vec3(1, 0.707, 0.707))
        ->rotate(90.0f, mgl::YY)
        ->rotate(45.0f, mgl::ZZ)
        ->translate(2, 0, 0);
    mgl::Transform* t_ocean_green_f = (new mgl::Transform())
        ->scale(glm::vec3(1, 0.5, 0.5))
        ->rotate(90.0f, mgl::YY)
        ->rotate(90.0f + 45.0f, mgl::ZZ)
        ->translate(1.414f, 2, 0);
    mgl::Transform* t_red_f = (new mgl::Transform())
        ->scale(glm::vec3(1, 0.5, 0.5))
        ->rotate(90.0f, mgl::YY)
        ->rotate(90.0f + 45.0f, mgl::ZZ)
        ->translate(-0.586, 0, 0);

    // scene objects
    mgl::SceneObject* pOrange = new mgl::SceneObject(MESHES[PARALLELOGRAM], ORANGE_M);
    pOrange->setTransform(p_orange_i);

    mgl::SceneObject* sGreen = new mgl::SceneObject(MESHES[SQUARE], GREEN_M);
    sGreen->setTransform(s_green_i);

    mgl::SceneObject* tPink = new mgl::SceneObject(MESHES[TRIANGLE], PINK_M);
    tPink->setTransform(t_pink_i);

    mgl::SceneObject* tBlue = new mgl::SceneObject(MESHES[TRIANGLE], BLUE_M);
    tBlue->setTransform(t_blue_i);    
    
    mgl::SceneObject* tPurple = new mgl::SceneObject(MESHES[TRIANGLE], PURPLE_M);
    tPurple->setTransform(t_purple_i);    
    
    mgl::SceneObject* tOceanGreen = new mgl::SceneObject(MESHES[TRIANGLE], OCEAN_GREEN_M);
    tOceanGreen->setTransform(t_ocean_green_i);

    mgl::SceneObject* tRed = new mgl::SceneObject(MESHES[TRIANGLE], RED_M);
    tRed->setTransform(t_red_i);

    // scene graph
    Scene = new mgl::SceneGraph(pOrange);
    Scene->add(sGreen);
    Scene->add(tPink);
    Scene->add(tBlue);
    Scene->add(tPurple);
    Scene->add(tOceanGreen);
    Scene->add(tRed);

    // animations
    ANIMATION[PARALLELOGRAM_ORANGE] = new mgl::Animation(pOrange, p_orange_f);
    ANIMATION[SQUARE_GREEN]         = new mgl::Animation(sGreen, s_green_f);
    ANIMATION[TRIANGLE_PINK]        = new mgl::Animation(tPink, t_pink_f);
    ANIMATION[TRIANGLE_BLUE]        = new mgl::Animation(tBlue, t_blue_f);
    ANIMATION[TRIANGLE_PURPLE]      = new mgl::Animation(tPurple, t_purple_f);
    ANIMATION[TRIANGLE_GREEN]       = new mgl::Animation(tOceanGreen, t_ocean_green_f);
    ANIMATION[TRIANGLE_RED]         = new mgl::Animation(tRed, t_red_f);
    ANIMATION[SCENE]                = new mgl::Animation(Scene, scene_f);
}

///////////////////////////////////////////////////////////////////////// INPUT


void MyApp::processInput(double elapsed) {
    // animation
    if (mgl::InputManager::getInstance().isKeyPressed(GLFW_KEY_RIGHT)) {
        for (const auto& animation : ANIMATION) {
            animation->step(elapsed * 8);
        }
    }
    if (mgl::InputManager::getInstance().isKeyPressed(GLFW_KEY_LEFT)) {
        for (const auto& animation : ANIMATION) {
            animation->step(-elapsed * 8);
        }
    }
    // camera movement
    if (mgl::InputManager::getInstance().isMouseBtnPressed(GLFW_MOUSE_BUTTON_LEFT)) {
        mgl::MouseMove movement = mgl::InputManager::getInstance().getMouseMovement();
        float val = -movement.xOffset * elapsed * 4;
        OrbitCams[currCam]->rotate(val, mgl::YY);
        float a = 0;
    }
}

void MyApp::registerInputCallBacks() {
    // toggle cameras
    mgl::InputManager::getInstance().registerKeyCallBack(GLFW_KEY_C, [this]() {
        currCam = (++currCam) % 2;
        OrbitCams[currCam]->setActive();
        });
    // toggle projection type
    mgl::InputManager::getInstance().registerKeyCallBack(GLFW_KEY_P, [this]() {
        if (OrbitCamPerspective[currCam])
            OrbitCams[currCam]->setOrtho(orthoP);
        else
            OrbitCams[currCam]->setPerspective(perspectiveP);

        OrbitCamPerspective[currCam] = !OrbitCamPerspective[currCam];
        });
}

void MyApp::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    OrbitCams[currCam]->zoom(yoffset * 0.3);
}


///////////////////////////////////////////////////////////////////////// CAMERAS

void MyApp::createCamera() {
    OrbitCams[0] = new OrbitCamera(glm::vec3(0, 0, 0), 5.0f, UBO_BP);
    OrbitCams[0]->setPerspective(perspectiveP);

    mgl::Camera::orthoResize(&orthoP, 800, 600);
    OrbitCams[1] = new OrbitCamera(glm::vec3(0, 0, 0), 5.0f, UBO_BP);
    OrbitCams[1]->setOrtho(orthoP);
    OrbitCams[1]->rotate(-90.0f, mgl::YY);

    currCam = 0;
    OrbitCams[0]->setActive(); // start with perspective
}

/////////////////////////////////////////////////////////////////////////// DRAW

void MyApp::drawScene() {
    Scene->draw();
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow* win) {
    registerInputCallBacks();
    createMeshes();
    createShaderPrograms();  // after mesh;
    createSceneGraph();
    createCamera();
}

void MyApp::windowSizeCallback(GLFWwindow* win, int winx, int winy) {
    glViewport(0, 0, winx, winy);
    // perspective camera resize when changing back to the camera
    mgl::Camera::perspectiveResize(&perspectiveP, winx, winy);
    mgl::Camera::orthoResize(&orthoP, winx, winy);
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
