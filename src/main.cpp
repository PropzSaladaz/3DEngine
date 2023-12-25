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
    mgl::Scene* Scene = nullptr;
    mgl::SceneGraph* lightObj;
    mgl::MeshManager* meshes;
    mgl::ShaderManager* shaders;
    mgl::Animation* squareAnim;
    // cameras
    mgl::FPSCamController* FPSCamera;
    mgl::OrbitCamController* OrbitCam;
    mgl::PerspectiveParams perspectiveP = { 45.0f, 800.0f / 600.0f, 0.1f, 100.0f };
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
    meshes->meshConfigCallback([](mgl::Mesh* mesh) {
        mesh->joinIdenticalVertices();
    });

    meshes->import("light", "resources/models/square.obj");
    meshes->import("statue", "resources/models/statue.obj");
    meshes->import("wood-base", "resources/models/base.obj");
    meshes->import("glass", "resources/models/dome.obj");
}

///////////////////////////////////////////////////////////////////////// SHADER

void MyApp::createShaderPrograms() {
    shaders = new mgl::ShaderManager();
    shaders->beforeBuild([this](mgl::ShaderProgram* program) {
        program->addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);
        program->addAttribute(mgl::NORMAL_ATTRIBUTE, mgl::Mesh::NORMAL);
        program->addAttribute(mgl::TEXCOORD_ATTRIBUTE, mgl::Mesh::TEXCOORD);
        program->addUniform(mgl::MODEL_MATRIX);
        program->addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);
        });

    mgl::ShaderProgram* statueShaders = new mgl::ShaderProgram();
    statueShaders->addShader(GL_VERTEX_SHADER, "src/shaders/vertexShader.glsl");
    statueShaders->addShader(GL_FRAGMENT_SHADER, "src/shaders/light/statue.glsl");
    statueShaders->addUniforms<mgl::PhongMaterial>();
    statueShaders->addUniforms<mgl::Light>();
    statueShaders->addUniform("texture1");
    statueShaders->addUniform("whiteColor");
    statueShaders->addUniform("darkColor");

    mgl::ShaderProgram* woodShaders = new mgl::ShaderProgram();
    woodShaders->addShader(GL_VERTEX_SHADER, "src/shaders/vertexShader.glsl");
    woodShaders->addShader(GL_FRAGMENT_SHADER, "src/shaders/light/statue.glsl");
    woodShaders->addUniforms<mgl::PhongMaterial>();
    woodShaders->addUniforms<mgl::Light>();
    woodShaders->addUniform("texture1");
    woodShaders->addUniform("whiteColor");
    woodShaders->addUniform("darkColor");

    mgl::ShaderProgram* glassShaders = new mgl::ShaderProgram();
    glassShaders->addShader(GL_VERTEX_SHADER, "src/shaders/vertexShader.glsl");
    glassShaders->addShader(GL_FRAGMENT_SHADER, "src/shaders/light/glass.glsl");
    glassShaders->addUniforms<mgl::PhongMaterial>();
    glassShaders->addUniforms<mgl::Light>();

    mgl::ShaderProgram* lightShaders = new mgl::ShaderProgram();
    lightShaders->addShader(GL_VERTEX_SHADER, "src/shaders/vertexShader.glsl");
    lightShaders->addShader(GL_FRAGMENT_SHADER, "src/shaders/light/basic-color-fs.glsl");
    lightShaders->addUniforms<mgl::BasicMaterial>();

    shaders->add("phong", statueShaders);
    shaders->add("light", lightShaders);
    shaders->add("wood", woodShaders);
    shaders->add("glass", glassShaders);
}

///////////////////////////////////////////////////////////////////////// SCENE
// Colors                     R       G       B       A 
const glm::vec4 RED         (0.600f, 0.360f, 0.200f, 1.0f);
const glm::vec3 lightColor(1, 1, 1);
const glm::vec4 position(0, 2, 4, 0);

void MyApp::createSceneGraph() {
    // light Object----------------------------------------------------
    mgl::Material* WHITE_M = new mgl::BasicMaterial(lightColor);
    mgl::Transform* light_pos = (new mgl::Transform())
        ->scale(0.03f)
        ->translate(0, 2.5f, 2);
    mgl::SceneObject* light = new mgl::SceneObject(
        meshes->get("light"), 
        WHITE_M, 
        shaders->get("light"));
    light->setTransform(light_pos);
    lightObj = new mgl::SceneGraph(light); // created to ease rotation (we rotate the center of graph, not the light itself)

    // statue ----------------------------------------------------
    mgl::Texture2D* t = new mgl::Texture2D();
    t->genSinePerlinNoise(512, 15, 200, 200, 15);
    mgl::Sampler* s = new mgl::LinearSampler();
    s->create();
    mgl::TextureInfo* tInfo = new mgl::TextureInfo(GL_TEXTURE0, 0, "texture1", t, s);

    mgl::Material* STONE_M = (new mgl::PhongMaterial())
        ->setAmbientColor(0.05f * mgl::COLOR_WHITE)
        ->setDiffuseColor(0.8f * mgl::COLOR_WHITE)
        ->setSpecularColor(0.9f * mgl::COLOR_WHITE)
        ->setShininess(64);
    STONE_M->setTexture(tInfo);

    mgl::Transform* statue_i = (new mgl::Transform())
        ->scale(0.01f)
        ->rotate(-90.0f, mgl::XX)
        ->translate(0, 0.15f, 0);
    mgl::Transform* statue_f = (new mgl::Transform())
        ->scale(glm::vec3(1, 0.5f, 0.5f))
        ->rotate(90.0f, mgl::YY);

    mgl::SceneObject* statueObj = new mgl::SceneObject(
        meshes->get("statue"), 
        STONE_M, 
        shaders->get("phong"));
    statueObj->setTransform(statue_i);

    statueObj->setShaderUniformCallback([](mgl::ShaderProgram* shaders) {
        shaders->setUniformVec4f("whiteColor", glm::value_ptr(glm::vec4(0.949f, 0.902f, 0.769f, 1.0f)));
        shaders->setUniformVec4f("darkColor", glm::value_ptr(glm::vec4(0.849f, 0.802f, 0.669f, 1.0f)));
        });

    // wooden base ----------------------------------------------------
    mgl::Texture2D* woodTexture = new mgl::Texture2D();
    woodTexture->genSawPerlinNoise(512, 10, 26, 2.6f);
    mgl::Sampler* woodSampler = new mgl::LinearSampler();
    s->create();

    mgl::TextureInfo* woodTInfo = new mgl::TextureInfo(GL_TEXTURE0, 0, "texture1", woodTexture, woodSampler);

    mgl::Material* WOOD_M = (new mgl::PhongMaterial())
        ->setAmbientColor(0.05f * mgl::COLOR_WHITE)
        ->setDiffuseColor(0.6f * mgl::COLOR_WHITE)
        ->setSpecularColor(0.6f * mgl::COLOR_WHITE)
        ->setShininess(17);
    WOOD_M->setTexture(woodTInfo);

    mgl::Transform* wood_i = (new mgl::Transform());

    mgl::SceneObject* woodenBaseObj = new mgl::SceneObject(
        meshes->get("wood-base"), 
        WOOD_M, 
        shaders->get("wood"));
    woodenBaseObj->setTransform(wood_i);
    woodenBaseObj->setShaderUniformCallback([](mgl::ShaderProgram* shaders) {
        shaders->setUniformVec4f("whiteColor", glm::value_ptr(glm::vec4(0.549f, 0.309f, 0.114f, 1.0f)));
        shaders->setUniformVec4f("darkColor", glm::value_ptr(glm::vec4(0.258f, 0.149f, 0.058f, 1.0f)));
        });

    // Glass dome ----------------------------------------------------
    // Has to be drawn last since it is transparent

    mgl::Material* GLASS_M = (new mgl::PhongMaterial(glm::vec4(mgl::COLOR_WHITE, 0.18f)))
        ->setDiffuseColor(glm::vec4(mgl::COLOR_WHITE, 0.01f))
        ->setSpecularColor(glm::vec4(mgl::COLOR_WHITE, 0.9f))
        ->setShininess(300);
    mgl::Transform* glass_pos = (new mgl::Transform())
        ->scale(glm::vec3(1.0f, 1.7f, 1.0f))
        ->translate(0, 0.1f, 0);
    mgl::SceneObject* glassObj = new mgl::SceneObject(
        meshes->get("glass"),
        GLASS_M,
        shaders->get("glass"));
    glassObj->setTransform(glass_pos);

    glassObj->beforeAndAfterDraw( // enable blending
        []() { // before
            //glDisable(GL_CULL_FACE); // front & back rendering TODO - try to implement OIT
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        },
        []() { // after
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glDisable(GL_BLEND);
        });

    // scene graph
    mgl::SceneGraph* graph = new mgl::SceneGraph(statueObj);
    graph->add(lightObj);
    graph->add(woodenBaseObj);
    graph->add(glassObj);

    Scene = new mgl::Scene(graph);

    mgl::PointLight* pointLight = new mgl::PointLight(light, mgl::COLOR_WHITE);
    pointLight->setAttenuation(50);

    mgl::SpotLight* spotLight = new mgl::SpotLight(light, mgl::COLOR_WHITE, glm::vec3(0.0f, 1.4f, 0.0f));
    spotLight->setAmbient(mgl::COLOR_WHITE * 0.0f);
    spotLight->setInnerCutoffAngle(2.0f);
    spotLight->setOuterCutoffAngle(20.0f);

    Scene->addCamera("main_camera", OrbitCam->getCamera());

    Scene->addLight("light1", pointLight);
    Scene->assignLightToCamera("light1", "main_camera");
    //Scene->addLight("spotLight", spotLight);
    //Scene->assignLightToCamera("spotLight", "main_camera");

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
    createCamera();
    createSceneGraph();
  
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
