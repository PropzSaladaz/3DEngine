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
    mgl::SceneGraph* lightObj;
    mgl::MeshManager* meshes;
    mgl::ShaderManager* shaders;
    mgl::Animation* squareAnim;
    // cameras
    mgl::FPSCamController* FPSCamera;
    mgl::OrbitCamController* OrbitCam;
    mgl::PerspectiveParams perspectiveP = { 45.0f, 800.0 / 600.0, 0.1, 100.0 };
    // input
    void processInput(double elapsed);
    void animateLights(double elapsed);
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

    meshes->import("light", "resources/models/square.obj");
    meshes->import("statue", "resources/models/statue.obj");
    meshes->import("wood-base", "resources/models/base.obj");
    //meshes->import("cube-vtn", "resources/models/sphere.obj");
}

///////////////////////////////////////////////////////////////////////// SHADER

void MyApp::createShaderPrograms() {
    shaders = new mgl::ShaderManager();
    shaders->setManagedItemCallback([this](mgl::ShaderProgram* program) {
        program->addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);
        program->addAttribute(mgl::NORMAL_ATTRIBUTE, mgl::Mesh::NORMAL);
        program->addAttribute(mgl::TEXCOORD_ATTRIBUTE, mgl::Mesh::TEXCOORD);
        program->addUniform(mgl::MODEL_MATRIX);
        program->addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);
        });

    mgl::ShaderProgram* statueShaders = new mgl::ShaderProgram();
    statueShaders->addShader(GL_VERTEX_SHADER, "src/shaders/vertexShader.glsl");
    statueShaders->addShader(GL_FRAGMENT_SHADER, "src/shaders/light/statue.glsl");
    statueShaders->addUniform(mgl::PhongMaterial::MATERIAL_PHONG_AMBIENT);
    statueShaders->addUniform(mgl::PhongMaterial::MATERIAL_PHONG_DIFFUSE);
    statueShaders->addUniform(mgl::PhongMaterial::MATERIAL_PHONG_SPECULAR);
    statueShaders->addUniform(mgl::PhongMaterial::MATERIAL_PHONG_SHININESS);
    statueShaders->addUniform("lightColor");
    statueShaders->addUniform("lightPos");
    statueShaders->addUniform("texture1");
    statueShaders->addUniform("whiteColor");
    statueShaders->addUniform("darkColor");

    mgl::ShaderProgram* woodShaders = new mgl::ShaderProgram();
    woodShaders->addShader(GL_VERTEX_SHADER, "src/shaders/vertexShader.glsl");
    woodShaders->addShader(GL_FRAGMENT_SHADER, "src/shaders/light/wood.glsl");
    woodShaders->addUniform(mgl::PhongMaterial::MATERIAL_PHONG_AMBIENT);
    woodShaders->addUniform(mgl::PhongMaterial::MATERIAL_PHONG_DIFFUSE);
    woodShaders->addUniform(mgl::PhongMaterial::MATERIAL_PHONG_SPECULAR);
    woodShaders->addUniform(mgl::PhongMaterial::MATERIAL_PHONG_SHININESS);
    woodShaders->addUniform("lightColor");
    woodShaders->addUniform("lightPos");
    woodShaders->addUniform("texture1");
    woodShaders->addUniform("whiteColor");
    woodShaders->addUniform("darkColor");

    mgl::ShaderProgram* lightShaders = new mgl::ShaderProgram();
    lightShaders->addShader(GL_VERTEX_SHADER, "src/shaders/vertexShader.glsl");
    lightShaders->addShader(GL_FRAGMENT_SHADER, "src/shaders/light/basic-color-fs.glsl");
    lightShaders->addUniform(mgl::COLOR_ATTRIBUTE);

    shaders->add("phong", statueShaders);
    shaders->add("light", lightShaders);
    shaders->add("wood", woodShaders);


}

///////////////////////////////////////////////////////////////////////// SCENE
// Colors                     R       G       B       A 
const glm::vec4 RED         (0.600f, 0.360f, 0.200f, 1.0f);
const glm::vec3 lightColor(1, 1, 1);
const glm::vec4 position(0, 2, 4, 0);

void MyApp::createSceneGraph() {
    // light ----------------------------------------------------
    mgl::Material* WHITE_M = (new mgl::BasicMaterial(shaders->get("light")))
        ->setColor(lightColor);
    mgl::Transform* light_i = (new mgl::Transform())
        ->scale(0.03)
        ->translate(0, 2.5, 2);
    mgl::SceneObject* light = new mgl::SceneObject(meshes->get("light"), WHITE_M);
    light->setTransform(light_i);
    lightObj = new mgl::SceneGraph(light);

    // statue ----------------------------------------------------
    mgl::Texture2D* t = new mgl::Texture2D();
    t->genSinePerlinNoise(512, 15, 200, 200, 15);
    mgl::Sampler* s = new mgl::LinearSampler();
    s->create();
    mgl::TextureInfo* tInfo = new mgl::TextureInfo(GL_TEXTURE0, 0, "texture1", t, s);

    mgl::Material* STONE_M = (new mgl::PhongMaterial(shaders->get("phong")))
        ->setAmbientColor(0.05f * mgl::COLOR_WHITE)
        ->setDiffuseColor(0.8f * mgl::COLOR_WHITE)
        ->setSpecularColor(0.9f * mgl::COLOR_WHITE)
        ->setShininess(256);
    STONE_M->setTexture(tInfo);

    mgl::Transform* statue_i = (new mgl::Transform())
        ->scale(0.01)
        ->rotate(-90.0f, mgl::XX)
        ->translate(0, 0.15, 0);
    mgl::Transform* statue_f = (new mgl::Transform())
        ->scale(glm::vec3(1, 0.5, 0.5))
        ->rotate(90.0f, mgl::YY);

    mgl::SceneObject* statueObj = new mgl::SceneObject(meshes->get("statue"), STONE_M);
    statueObj->setTransform(statue_i);
    statueObj->setShaderUniformCallback([light](mgl::ShaderProgram* shaders) {
        shaders->setUniformVec3f("lightColor", glm::value_ptr(lightColor));
        shaders->setUniformVec3f("lightPos", glm::value_ptr(light->getAbsolutePosition()));
        shaders->setUniformVec3f("whiteColor", glm::value_ptr(glm::vec3(0.949, 0.902, 0.769)));
        shaders->setUniformVec3f("darkColor", glm::value_ptr(glm::vec3(0.849, 0.802, 0.669)));
        });

    // wooden base ----------------------------------------------------
    mgl::Texture2D* woodTexture = new mgl::Texture2D();
    woodTexture->genSawPerlinNoise(512, 10, 26, 2.6);
    mgl::Sampler* woodSampler = new mgl::LinearSampler();
    s->create();

    mgl::TextureInfo* woodTInfo = new mgl::TextureInfo(GL_TEXTURE0, 0, "texture1", woodTexture, woodSampler);

    mgl::Material* WOOD_M = (new mgl::PhongMaterial(shaders->get("wood")))
        ->setAmbientColor(0.05f * mgl::COLOR_WHITE)
        ->setDiffuseColor(0.6f * mgl::COLOR_WHITE)
        ->setSpecularColor(0.6f * mgl::COLOR_WHITE)
        ->setShininess(17);
    WOOD_M->setTexture(woodTInfo);

    mgl::Transform* wood_i = (new mgl::Transform());

    mgl::SceneObject* woodenBaseObj = new mgl::SceneObject(meshes->get("wood-base"), WOOD_M);
    woodenBaseObj->setTransform(wood_i);
    woodenBaseObj->setShaderUniformCallback([light](mgl::ShaderProgram* shaders) {
        shaders->setUniformVec3f("lightColor", glm::value_ptr(lightColor));
        shaders->setUniformVec3f("lightPos", glm::value_ptr(light->getAbsolutePosition()));
        shaders->setUniformVec3f("whiteColor", glm::value_ptr(glm::vec3(0.549, 0.309, 0.114)));
        shaders->setUniformVec3f("darkColor", glm::value_ptr(glm::vec3(0.258, 0.149, 0.058)));
        });

    // scene graph
    Scene = new mgl::SceneGraph(statueObj);
    Scene->add(lightObj);
    Scene->add(woodenBaseObj);
    //Scene->registerCallback(
    //    []() { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }, // init
    //    []() { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }  // destroy
    //);

    // animations
    squareAnim = new mgl::Animation(statueObj, statue_f);
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
    //FPSCamera = new mgl::FPSCamController(camera, 5, 0.1);
    //FPSCamera->setPosition(0, 0, 5);
    //FPSCamera->setActive();

    OrbitCam = new mgl::OrbitCamController(camera, glm::vec3(0, 0, 0), 5.0f);
    OrbitCam->setActive();
}

/////////////////////////////////////////////////////////////////////////// DRAW

void MyApp::animateLights(double elapsed) {
    lightObj->rotate(elapsed * 15, mgl::YY);
}


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
    animateLights(elapsed);
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
