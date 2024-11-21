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
    mgl::SceneGraph *lightObj, *lightObj2;
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

    meshes->import("light", "resources/models/sphere.obj");
    meshes->import("statue", "resources/models/statue.obj");
    meshes->import("wood-base", "resources/models/base.obj");
    meshes->import("glass", "resources/models/dome3.obj");
}

///////////////////////////////////////////////////////////////////////// TEXTURES

void MyApp::createTextures() {
    textures = new mgl::TextureManager();

    // stone texture
    mgl::Texture2D* stoneT = new mgl::Texture2D();
    stoneT->genSinePerlinNoise(512, 15, 200, 200, 15);
    mgl::Sampler* stoneS = new mgl::LinearSampler();
    stoneS->create();
    mgl::TextureInfo* stoneTinfo = new mgl::TextureInfo(GL_TEXTURE1, 1, "texture1", stoneT, stoneS);

    // enviroment diffuse lighting
    mgl::TextureCubeMap* environmentDiffuseT = new mgl::TextureCubeMap();
    environmentDiffuseT->loadCubeMap("resources/textures/cubemaps/bedroom_diffuse/", "png");
    mgl::Sampler* environmentDiffuseS = new mgl::LinearSampler();
    environmentDiffuseS->create();
    mgl::TextureInfo* environmentDiffuseTinfo = new mgl::TextureInfo(GL_TEXTURE2, 2,
        "DiffuseTexture", environmentDiffuseT, environmentDiffuseS);

    // enviroment diffuse lighting
    mgl::TextureCubeMap* environmentSpecT = new mgl::TextureCubeMap();
    environmentSpecT->loadCubeMap("resources/textures/cubemaps/bedroom_spec/", "png");
    mgl::Sampler* environmentSpecS = new mgl::LinearSampler();
    environmentSpecS->create();
    mgl::TextureInfo* environmentSpecTinfo = new mgl::TextureInfo(GL_TEXTURE3, 3,
        "SpecularTexture", environmentSpecT, environmentSpecS);

    // wood texture
    mgl::Texture2D* woodT = new mgl::Texture2D();
    woodT->genSawPerlinNoise(512, 10, 26, 2.6f);
    mgl::Sampler* woodS = new mgl::LinearSampler();
    woodS->create();
    mgl::TextureInfo* woodTinfo = new mgl::TextureInfo(GL_TEXTURE1, 1, "texture1", woodT, woodS);

    textures->add("stone", stoneTinfo);
    textures->add("wood", woodTinfo);
    textures->add("environmentDiffuse", environmentDiffuseTinfo);
    textures->add("environmentSpecular", environmentSpecTinfo);
}

// Colors                          R       G       B      
const glm::vec3 LIGHT_COLOR_1   (0.912f, 0.224f, 0.256f);
const glm::vec3 LIGHT_COLOR_2   (0.955f, 0.943f, 0.555f);
const glm::vec3 LIGHT_COLOR_3   (1, 1, 1);

const glm::vec4 STATUE_COLOR_1(0.949f, 0.902f, 0.769f, 1.0f);
const glm::vec4 STATUE_COLOR_2(0.849f, 0.802f, 0.669f, 1.0f);

const glm::vec4 WOOD_COLOR_1(0.549f, 0.309f, 0.114f, 1.0f);
const glm::vec4 WOOD_COLOR_2(0.258f, 0.149f, 0.058f, 1.0f);

///////////////////////////////////////////////////////////////////////// MATERIALS

void MyApp::createMaterials() {
    materials = new mgl::MaterialManager();
    // stone
    mgl::Material* STONE_M = (new mgl::PhongMaterial())
        ->setAmbientColor(0.8f * LIGHT_COLOR_3)
        ->setDiffuseColor(0.8f * LIGHT_COLOR_3)
        ->setSpecularColor(0.9f * LIGHT_COLOR_3)
        ->setShininess(512);
    STONE_M->addTexture(textures->get("stone"));
    STONE_M->addTexture(textures->get("environmentDiffuse"));
    STONE_M->addTexture(textures->get("environmentSpecular"));
    // light
    mgl::Material* LIGHT_1_M = new mgl::BasicMaterial(glm::vec3(0.912f, 0.824f, 0.856f));
    mgl::Material* LIGHT_2_M = new mgl::BasicMaterial(LIGHT_COLOR_2);
    // wood
    mgl::Material* WOOD_M = (new mgl::PhongMaterial())
        ->setAmbientColor(0.8f * LIGHT_COLOR_3)
        ->setDiffuseColor(0.6f * LIGHT_COLOR_3)
        ->setSpecularColor(0.6f * LIGHT_COLOR_3)
        ->setShininess(17);
    WOOD_M->addTexture(textures->get("wood"));
    WOOD_M->addTexture(textures->get("environmentDiffuse"));
    WOOD_M->addTexture(textures->get("environmentSpecular"));
    // glass
    mgl::Material* GLASS_M = (new mgl::PhongMaterial(glm::vec4(mgl::COLOR_WHITE, 0.1f)))
        ->setDiffuseColor(glm::vec4(mgl::COLOR_WHITE, 0.0f))
        ->setSpecularColor(glm::vec4(mgl::COLOR_WHITE, 0.9f))
        ->setShininess(500);

    materials->add("stone", STONE_M);
    materials->add("light1", LIGHT_1_M);
    materials->add("light2", LIGHT_2_M);
    materials->add("wood", WOOD_M);
    materials->add("glass", GLASS_M);
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
    statueShaders->addShader(GL_VERTEX_SHADER, "resources/shaders/vertexShader.glsl");
    statueShaders->addShader(GL_FRAGMENT_SHADER, "resources/shaders/light/statue.glsl");
    statueShaders->addUniforms<mgl::PhongMaterial>();
    statueShaders->addUniforms<mgl::Light>();
    statueShaders->addUniform(mgl::SKYBOX);
    statueShaders->addUniform("texture1");
    statueShaders->addUniform("whiteColor");
    statueShaders->addUniform("darkColor");
    statueShaders->addUniform("DiffuseTexture");
    statueShaders->addUniform("SpecularTexture");

    mgl::ShaderProgram* woodShaders = new mgl::ShaderProgram();
    woodShaders->addShader(GL_VERTEX_SHADER, "resources/shaders/vertexShader.glsl");
    woodShaders->addShader(GL_FRAGMENT_SHADER, "resources/shaders/light/statue.glsl");
    woodShaders->addUniforms<mgl::PhongMaterial>();
    woodShaders->addUniforms<mgl::Light>();
    woodShaders->addUniform(mgl::SKYBOX);
    woodShaders->addUniform("texture1");
    woodShaders->addUniform("whiteColor");
    woodShaders->addUniform("darkColor");
    woodShaders->addUniform("DiffuseTexture");
    woodShaders->addUniform("SpecularTexture");

    mgl::ShaderProgram* glassFrontShaders = new mgl::ShaderProgram();
    glassFrontShaders->addShader(GL_VERTEX_SHADER, "resources/shaders/vertexShader.glsl");
    glassFrontShaders->addShader(GL_FRAGMENT_SHADER, "resources/shaders/light/glass-front.glsl");
    glassFrontShaders->addUniforms<mgl::PhongMaterial>();
    glassFrontShaders->addUniforms<mgl::Light>();
    glassFrontShaders->addUniform(mgl::SKYBOX);

    mgl::ShaderProgram* glassBackShaders = new mgl::ShaderProgram();
    glassBackShaders->addShader(GL_VERTEX_SHADER, "resources/shaders/vertexShader.glsl");
    glassBackShaders->addShader(GL_FRAGMENT_SHADER, "resources/shaders/light/glass-back.glsl");
    glassBackShaders->addUniforms<mgl::PhongMaterial>();
    glassBackShaders->addUniforms<mgl::Light>();
    glassBackShaders->addUniform(mgl::SKYBOX);

    mgl::ShaderProgram* lightShaders = new mgl::ShaderProgram();
    lightShaders->addShader(GL_VERTEX_SHADER, "resources/shaders/vertexShader.glsl");
    lightShaders->addShader(GL_FRAGMENT_SHADER, "resources/shaders/light/basic-color-fs.glsl");
    lightShaders->addUniforms<mgl::BasicMaterial>();

    shaders->add("phong", statueShaders);
    shaders->add("light", lightShaders);
    shaders->add("wood", woodShaders);
    shaders->add("glass-front", glassFrontShaders);
    shaders->add("glass-back", glassBackShaders);
}

///////////////////////////////////////////////////////////////////////// SCENE


void MyApp::createSceneGraph() {
     // light Object----------------------------------------------------
    mgl::SceneObject* light = new mgl::SceneObject(
        meshes->get("light"), 
        materials->get("light1"),
        shaders->get("light"));
    light->setTransform((new mgl::Transform())
        ->scale(0.03f)
        ->translate(0, 0.4f, 0.65f));
    lightObj = new mgl::SceneGraph(light); // created to ease rotation (we rotate the center of graph, not the light itself)


    mgl::SceneObject* light2 = new mgl::SceneObject(
        meshes->get("light"),
        materials->get("light2"),
        shaders->get("light"));
    light2->setTransform((new mgl::Transform())
        ->scale(0.03f)
        ->translate(0, 0.8f, -0.65f));
    lightObj2 = new mgl::SceneGraph(light2); // created to ease rotation (we rotate the center of graph, not the light itself)

    // statue ----------------------------------------------------
    mgl::SceneObject* statueObj = new mgl::SceneObject(
        meshes->get("statue"), 
        materials->get("stone"),
        shaders->get("phong"));
    statueObj->setTransform((new mgl::Transform())
        ->scale(0.01f)
        ->rotate(-90.0f, mgl::XX)
        ->translate(0, 0.15f, 0));
    statueObj->setShaderUniformCallback([](mgl::ShaderProgram* shaders) {
        shaders->setUniformVec4f("whiteColor", glm::value_ptr(STATUE_COLOR_1));
        shaders->setUniformVec4f("darkColor", glm::value_ptr(STATUE_COLOR_2));
        });

    // wooden base ----------------------------------------------------
    mgl::SceneObject* woodenBaseObj = new mgl::SceneObject(
        meshes->get("wood-base"), 
        materials->get("wood"),
        shaders->get("wood"));
    woodenBaseObj->setShaderUniformCallback([](mgl::ShaderProgram* shaders) {
        shaders->setUniformVec4f("whiteColor", glm::value_ptr(WOOD_COLOR_1));
        shaders->setUniformVec4f("darkColor", glm::value_ptr(WOOD_COLOR_2));
        });

    // Glass dome ----------------------------------------------------
    // Has to be created to be drawn last (graph objects ordered by id,
    // which is incremented for each new object)
    mgl::SceneObject* glassBackObj = new mgl::SceneObject(
        meshes->get("glass"),
        materials->get("glass"),
        shaders->get("glass-back"));

    glassBackObj->beforeAndAfterDraw( // enable blending
        []() { // before
            glCullFace(GL_FRONT);
        },
        []() { // after
            glCullFace(GL_BACK);
        });

    mgl::SceneObject* glassFrontObj = new mgl::SceneObject(
        meshes->get("glass"),
        materials->get("glass"),
        shaders->get("glass-front"));

    glassFrontObj->beforeAndAfterDraw( // enable blending
        []() { // before
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        },
        []() { // after
            glDisable(GL_BLEND);
        });

    mgl::SceneGraph* dome = new mgl::SceneGraph(glassBackObj);
    dome->add(glassFrontObj);
    mgl::Transform* dome_i = (new mgl::Transform())
        ->scale(0.9f)
        ->translate(0, 0.1f, 0);
    mgl::Transform* dome_f = (new mgl::Transform(dome_i))
        ->rotate(15.0f, dome_i->getRightV())
        ->translate(0.0f, 3.0f, 0.0f);
    dome->setTransform(dome_i);

    // scene graph
    mgl::SceneGraph* graph = new mgl::SceneGraph(woodenBaseObj);
    graph->add(lightObj);
    graph->add(lightObj2);
    graph->add(statueObj);
    graph->add(dome);

    Scene = new mgl::Scene(meshes, shaders, textures);
    Scene->setScenegraph(graph);
    Scene->setSkybox("resources/textures/cubemaps/bedroom/", "png");

    // lights
    mgl::PointLight* pointLight = new mgl::PointLight(light, LIGHT_COLOR_1 * 1.0f);
    pointLight->setAmbient(LIGHT_COLOR_1 * 0.1f);
    pointLight->setAttenuation(10);
    mgl::SpotLight* spotLight = new mgl::SpotLight(light2, LIGHT_COLOR_2 * 1.0f, glm::vec3(0.0f, 2.0f, 0.0f));
    spotLight->setAmbient(LIGHT_COLOR_2 * 0.0f);
    spotLight->setInnerCutoffAngle(15.0f);
    spotLight->setOuterCutoffAngle(17.0f);
    mgl::DirectionalLight* dirLight = new mgl::DirectionalLight(glm::vec3(0, -1, -1), LIGHT_COLOR_3 * 1.0f);
    dirLight->setSpecular(LIGHT_COLOR_3 * 0.55f); // since the environment has cloudy sun
    dirLight->setAmbient(LIGHT_COLOR_3 * 0.4f);

    Scene->addCamera("main_camera", OrbitCam->getCamera()); 
    Scene->addLight("light1", pointLight);
    Scene->assignLightToCamera("light1", "main_camera");
    Scene->addLight("spotLight", spotLight);
    Scene->assignLightToCamera("spotLight", "main_camera");
    Scene->addLight("dirLight", dirLight);
    Scene->assignLightToCamera("dirLight", "main_camera");


    Animation = new mgl::Animation(dome, dome_f);

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

void MyApp::windowSizeCallback(GLFWwindow *window, int winx, int winy) {
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
