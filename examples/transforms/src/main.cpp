#include "mgl/scene/mglSceneGraph.hpp"
#include <memory>
#include <mgl/mgl.hpp>
#include <mgl/mglInputManager.hpp>
#include <vector>

////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App {
public:
  void onRegisterGlobalResources(mgl::ResourceContext &resources) override;
  void onCreateScene(mgl::Scene &scene,
                     mgl::ResourceContext &resources) override;
  void onUpdate(double deltaTime) override;

private:
  const float initialScreenRatio = 800.0f / 600.0f;
  const GLuint UBO_BP = 0;
  std::shared_ptr<mgl::FPSCamController> FPSCamera;
  GLuint cam = 0;
  mgl::PerspectiveParams perspectiveP = {60.0f, 800.0f / 600.0f, 0.1f, 400.0f};
  std::shared_ptr<mgl::Texture2D> checkerTexture;

  // scene
  void createMeshes(mgl::MeshManager &meshManager);
  void createTextures(mgl::TextureManager &textureManager);
  void createMaterials(mgl::MaterialManager &materialManager);
  void createShaderPrograms(mgl::ShaderManager &shaderManager);
  std::shared_ptr<mgl::SceneGraph> createShip(mgl::ResourceContext &resources);

  // texture UV coordinates moving
  float time = 0.0f;

  const uint DNA_HELIX_CUBE_COUNT = 1000;
  const uint DNA_HELIX_CUBE_SPACING = 1;
  const uint DNA_HELIX_RADIUS = 10;
  const uint ROTATION_SPEED = 50;

  std::vector<std::shared_ptr<mgl::SceneObject>> dnaHelix1;
  std::vector<std::shared_ptr<mgl::SceneObject>> dnaHelix2;
  std::shared_ptr<mgl::SceneGraph> helix;
  std::shared_ptr<mgl::SceneGraph> ship;
};

///////////////////////////////////////////////////////////////////////// MESHES

void MyApp::createMeshes(mgl::MeshManager &meshManager) {
  std::shared_ptr<mgl::Mesh> cube = std::make_shared<mgl::Mesh>();

  mgl::MeshData cubeData = {
      // we need 24 vertices because we are texturing each face of the cube
      // and each face has 4 vertices
      .positions =
          {
              // Front face
              mgl::vec3(-0.5f, -0.5f, 0.5f),
              mgl::vec3(0.5f, -0.5f, 0.5f),
              mgl::vec3(0.5f, 0.5f, 0.5f),
              mgl::vec3(-0.5f, 0.5f, 0.5f),
              // Back face
              mgl::vec3(0.5f, -0.5f, -0.5f),
              mgl::vec3(-0.5f, -0.5f, -0.5f),
              mgl::vec3(-0.5f, 0.5f, -0.5f),
              mgl::vec3(0.5f, 0.5f, -0.5f),
              // Top face
              mgl::vec3(-0.5f, 0.5f, 0.5f),
              mgl::vec3(0.5f, 0.5f, 0.5f),
              mgl::vec3(0.5f, 0.5f, -0.5f),
              mgl::vec3(-0.5f, 0.5f, -0.5f),
              // Bottom face
              mgl::vec3(-0.5f, -0.5f, -0.5f),
              mgl::vec3(0.5f, -0.5f, -0.5f),
              mgl::vec3(0.5f, -0.5f, 0.5f),
              mgl::vec3(-0.5f, -0.5f, 0.5f),
              // Right face
              mgl::vec3(0.5f, -0.5f, 0.5f),
              mgl::vec3(0.5f, -0.5f, -0.5f),
              mgl::vec3(0.5f, 0.5f, -0.5f),
              mgl::vec3(0.5f, 0.5f, 0.5f),
              // Left face
              mgl::vec3(-0.5f, -0.5f, -0.5f),
              mgl::vec3(-0.5f, -0.5f, 0.5f),
              mgl::vec3(-0.5f, 0.5f, 0.5f),
              mgl::vec3(-0.5f, 0.5f, -0.5f),
          },
      .indices =
          {
              0,  1,  2,  2,  3,  0,  // Front
              4,  5,  6,  6,  7,  4,  // Back
              8,  9,  10, 10, 11, 8,  // Top
              12, 13, 14, 14, 15, 12, // Bottom
              16, 17, 18, 18, 19, 16, // Right
              20, 21, 22, 22, 23, 20, // Left
          },
      .texcoords =
          {
              // Same pattern for each face: corners of texture
              mgl::vec2(0.0f, 0.0f), mgl::vec2(1.0f, 0.0f),
              mgl::vec2(1.0f, 1.0f), mgl::vec2(0.0f, 1.0f), // Front
              mgl::vec2(0.0f, 0.0f), mgl::vec2(1.0f, 0.0f),
              mgl::vec2(1.0f, 1.0f), mgl::vec2(0.0f, 1.0f), // Back
              mgl::vec2(0.0f, 0.0f), mgl::vec2(1.0f, 0.0f),
              mgl::vec2(1.0f, 1.0f), mgl::vec2(0.0f, 1.0f), // Top
              mgl::vec2(0.0f, 0.0f), mgl::vec2(1.0f, 0.0f),
              mgl::vec2(1.0f, 1.0f), mgl::vec2(0.0f, 1.0f), // Bottom
              mgl::vec2(0.0f, 0.0f), mgl::vec2(1.0f, 0.0f),
              mgl::vec2(1.0f, 1.0f), mgl::vec2(0.0f, 1.0f), // Right
              mgl::vec2(0.0f, 0.0f), mgl::vec2(1.0f, 0.0f),
              mgl::vec2(1.0f, 1.0f), mgl::vec2(0.0f, 1.0f), // Left
          },
  };
  cube->createFromData(cubeData);
  meshManager.add("cube", cube);
}

void MyApp::createTextures(mgl::TextureManager &manager) {
  // 2x2 checkerboard
  checkerTexture = std::make_shared<mgl::Texture2D>();
  checkerTexture->genCheckerboard(64, 64, 2);
  manager.add("checkerboard2x2", checkerTexture);

  // 8x8 checkerboard
  checkerTexture = std::make_shared<mgl::Texture2D>();
  checkerTexture->genCheckerboard(64, 64, 8);
  manager.add("checkerboard8x8", checkerTexture);
}

///////////////////////////////////////////////////////////////////////// SHADER

void MyApp::createShaderPrograms(mgl::ShaderManager &shaderManager) {
  // Simple shader
  mgl::ShaderBuilder simpleShaders;
  simpleShaders.addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);
  simpleShaders.addAttribute(mgl::TEXCOORD_ATTRIBUTE, mgl::Mesh::TEXCOORD);
  simpleShaders.addShader(GL_VERTEX_SHADER, "shaders/vert.glsl");
  simpleShaders.addShader(GL_FRAGMENT_SHADER, "shaders/frag.glsl");
  simpleShaders.addUniforms<mgl::BasicMaterial>();
  simpleShaders.addUniform(mgl::MODEL_MATRIX);
  simpleShaders.addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);

  simpleShaders.addUniform("texture1");

  std::shared_ptr<mgl::ShaderProgram> program =
      std::make_shared<mgl::ShaderProgram>(simpleShaders.build());
  shaderManager.add("textured", program);
}

///////////////////////////////////////////////////////////////////////// SCENE

void MyApp::onCreateScene(mgl::Scene &scene, mgl::ResourceContext &resources) {
  // create the camera
  auto cam = std::make_shared<mgl::PerspectiveCamera>(UBO_BP, perspectiveP);
  cam->setPosition(0.f, 0.f, 12.f);

  // target to look at the origin
  mgl::Transform target;
  target.setPosition(0.f, 0.f, 0.f);
  cam->lookAt(&target);
  scene.addCamera("mainCam", cam);

  cam->setActive();

  // build cam controller
  FPSCamera = std::make_shared<mgl::FPSCamController>(cam.get());
  FPSCamera->setMovementSpeed(10.0f);
  FPSCamera->setMouseSensitivity(0.1f);
  FPSCamera->setActive();

  auto cubeMesh = resources.meshManager.get("cube");
  auto shader = resources.shaderManager.get("textured");

  std::shared_ptr<mgl::SceneGraph> helix = std::make_shared<mgl::SceneGraph>();

  auto computeDnaHelixPosition = [](float offset, float index, float radius,
                                    float spacing) {
    float x = radius * cos(offset + index);
    float y = radius * sin(offset + index);
    float z = spacing * index;
    return mgl::vec3(x, y, -z);
  };

  float offset = 0.0f;
  // Build helix 1
  for (uint i = 0; i < DNA_HELIX_CUBE_COUNT; i++) {
    auto cubeObj = std::make_shared<mgl::SceneObject>(cubeMesh);
    cubeObj->setShaders(shader);

    if (i % 2 == 0) {
      cubeObj->setMaterial(resources.materialManager.get("checkerboard2x2"));
    } else {
      cubeObj->setMaterial(resources.materialManager.get("checkerboard8x8"));
    }

    cubeObj->setPosition(computeDnaHelixPosition(offset, i, DNA_HELIX_RADIUS,
                                                 DNA_HELIX_CUBE_SPACING));
    helix->add(cubeObj);
    dnaHelix1.push_back(cubeObj);
  }

  // Build helix 2
  offset = mgl::math::PI;
  for (uint i = 0; i < DNA_HELIX_CUBE_COUNT; i++) {
    auto cubeObj = std::make_shared<mgl::SceneObject>(cubeMesh);
    cubeObj->setShaders(shader);

    if (i % 2 != 0) {
      cubeObj->setMaterial(resources.materialManager.get("checkerboard2x2"));
    } else {
      cubeObj->setMaterial(resources.materialManager.get("checkerboard8x8"));
    }

    cubeObj->setPosition(computeDnaHelixPosition(offset, i, DNA_HELIX_RADIUS,
                                                 DNA_HELIX_CUBE_SPACING));
    helix->add(cubeObj);
    dnaHelix2.push_back(cubeObj);
  }

  this->helix = helix;

  // Create the ship with parent-child hierarchy
  ship = createShip(resources);
  ship->setPosition(30.0f, 0.0f, -50.0f); // Position ship to the side

  auto graph = std::make_shared<mgl::SceneGraph>();
  graph->add(helix);
  graph->add(ship);
  scene.setScenegraph(graph);
}

/////////////////////////////////////////////////////////////////////////
/// SPACESHIP
// Creates a spaceship using cube primitives with parent-child hierarchy
// Structure:
//   Spaceship (root SceneGraph)
//   ├── Fuselage (elongated body)
//   ├── Cockpit (front section)
//   ├── LeftWing (SceneGraph)
//   │   ├── WingBody
//   │   └── WingTip
//   ├── RightWing (SceneGraph)
//   │   ├── WingBody
//   │   └── WingTip
//   ├── TailFin (vertical stabilizer)
//   └── Engines (SceneGraph)
//       ├── LeftEngine
//       └── RightEngine

std::shared_ptr<mgl::SceneGraph>
MyApp::createShip(mgl::ResourceContext &resources) {
  auto cubeMesh = resources.meshManager.get("cube");
  auto shader = resources.shaderManager.get("textured");
  auto material2x2 = resources.materialManager.get("checkerboard2x2");
  auto material8x8 = resources.materialManager.get("checkerboard8x8");

  // Helper to create a cube with material
  auto makeCube = [&](std::shared_ptr<mgl::Material> mat) {
    auto obj = std::make_shared<mgl::SceneObject>(cubeMesh);
    obj->setShaders(shader);
    obj->setMaterial(mat);
    return obj;
  };

  // ===== SPACESHIP ROOT =====
  auto shipRoot = std::make_shared<mgl::SceneGraph>();

  // ===== FUSELAGE (main body) =====
  auto fuselage = makeCube(material8x8);
  fuselage->setScale(mgl::vec3(1.5f, 1.0f, 6.0f)); // Sleek elongated body
  fuselage->setPosition(0.0f, 0.0f, 0.0f);
  shipRoot->add(fuselage);

  // ===== COCKPIT (front) =====
  auto cockpit = makeCube(material2x2);
  cockpit->setScale(mgl::vec3(1.0f, 0.6f, 1.5f)); // Smaller front section
  cockpit->setPosition(0.0f, 0.2f, 3.5f);         // At front of fuselage
  shipRoot->add(cockpit);

  // ===== LEFT WING =====
  auto leftWing = std::make_shared<mgl::SceneGraph>();
  leftWing->setPosition(-1.5f, -0.2f, -0.5f); // Left side, slightly back
  leftWing->rotate(-10.0f, mgl::vec3(0.0f, 0.0f, 1.0f)); // Slight angle

  auto leftWingBody = makeCube(material8x8);
  leftWingBody->setScale(mgl::vec3(3.0f, 0.15f, 2.0f)); // Flat wing
  leftWingBody->setPosition(-1.5f, 0.0f, 0.0f);
  leftWing->add(leftWingBody);

  auto leftWingTip = makeCube(material2x2);
  leftWingTip->setScale(mgl::vec3(0.8f, 0.3f, 0.8f)); // Wing tip
  leftWingTip->setPosition(-3.0f, 0.0f, 0.0f);
  leftWing->add(leftWingTip);

  shipRoot->add(leftWing);

  // ===== RIGHT WING =====
  auto rightWing = std::make_shared<mgl::SceneGraph>();
  rightWing->setPosition(1.5f, -0.2f, -0.5f); // Right side, slightly back
  rightWing->rotate(10.0f, mgl::vec3(0.0f, 0.0f, 1.0f)); // Slight angle

  auto rightWingBody = makeCube(material8x8);
  rightWingBody->setScale(mgl::vec3(3.0f, 0.15f, 2.0f)); // Flat wing
  rightWingBody->setPosition(1.5f, 0.0f, 0.0f);
  rightWing->add(rightWingBody);

  auto rightWingTip = makeCube(material2x2);
  rightWingTip->setScale(mgl::vec3(0.8f, 0.3f, 0.8f)); // Wing tip
  rightWingTip->setPosition(3.0f, 0.0f, 0.0f);
  rightWing->add(rightWingTip);

  shipRoot->add(rightWing);

  // ===== TAIL FIN (vertical stabilizer) =====
  auto tailFin = makeCube(material2x2);
  tailFin->setScale(mgl::vec3(0.15f, 1.5f, 1.5f)); // Tall vertical fin
  tailFin->setPosition(0.0f, 0.8f, -2.5f);         // At back, on top
  shipRoot->add(tailFin);

  // ===== ENGINES =====
  auto engines = std::make_shared<mgl::SceneGraph>();
  engines->setPosition(0.0f, -0.3f, -3.2f); // At the back

  auto leftEngine = makeCube(material8x8);
  leftEngine->setScale(mgl::vec3(0.6f, 0.6f, 1.2f)); // Cylindrical-ish engine
  leftEngine->setPosition(-0.8f, 0.0f, 0.0f);
  engines->add(leftEngine);

  auto rightEngine = makeCube(material8x8);
  rightEngine->setScale(mgl::vec3(0.6f, 0.6f, 1.2f));
  rightEngine->setPosition(0.8f, 0.0f, 0.0f);
  engines->add(rightEngine);

  // Engine exhaust glow (small cubes at back of engines)
  auto leftExhaust = makeCube(material2x2);
  leftExhaust->setScale(mgl::vec3(0.4f, 0.4f, 0.2f));
  leftExhaust->setPosition(-0.8f, 0.0f, -0.7f);
  engines->add(leftExhaust);

  auto rightExhaust = makeCube(material2x2);
  rightExhaust->setScale(mgl::vec3(0.4f, 0.4f, 0.2f));
  rightExhaust->setPosition(0.8f, 0.0f, -0.7f);
  engines->add(rightExhaust);

  shipRoot->add(engines);

  return shipRoot;
}

void MyApp::createMaterials(mgl::MaterialManager &materialManager) {

  // 2x2 checkerboard
  auto checkerboardTexture =
      mgl::Engine::getInstance().getTextureManager().get("checkerboard2x2");
  auto checkerboard2x2Sampler = std::make_shared<mgl::Sampler>();
  checkerboard2x2Sampler->create(mgl::TextureFilter::Linear,
                                 mgl::TextureFilter::Linear);
  auto checkerboard2x2TexInfo = std::make_shared<mgl::TextureSampler>(
      GL_TEXTURE0, 0, "texture1", checkerboardTexture, checkerboard2x2Sampler);

  auto checkerboard2x2Material =
      std::make_shared<mgl::BasicMaterial>(mgl::vec4(1.0f));
  checkerboard2x2Material->addTexture(checkerboard2x2TexInfo);
  materialManager.add("checkerboard2x2", checkerboard2x2Material);

  // 8x8 checkerboard
  auto checkerboardTexture8x8 =
      mgl::Engine::getInstance().getTextureManager().get("checkerboard8x8");
  auto checkerboard8x8Sampler = std::make_shared<mgl::Sampler>();
  checkerboard8x8Sampler->create(mgl::TextureFilter::Linear,
                                 mgl::TextureFilter::Linear);
  auto checkerboard8x8TexInfo = std::make_shared<mgl::TextureSampler>(
      GL_TEXTURE0, 0, "texture1", checkerboardTexture8x8,
      checkerboard8x8Sampler);

  auto checkerboard8x8Material =
      std::make_shared<mgl::BasicMaterial>(mgl::vec4(1.0f));
  checkerboard8x8Material->addTexture(checkerboard8x8TexInfo);
  materialManager.add("checkerboard8x8", checkerboard8x8Material);
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::onRegisterGlobalResources(mgl::ResourceContext &resources) {
  createMeshes(resources.meshManager);
  createTextures(resources.textureManager);
  createMaterials(resources.materialManager);
  createShaderPrograms(resources.shaderManager); // after mesh;

  mgl::InputManager::getInstance().registerKeyCallBack(GLFW_KEY_TAB, [this]() {
    mgl::Engine::getInstance().toggleCursorCapture();
  });
}

void MyApp::onUpdate(double deltaTime) {

  // update time for texture UV coordinates moving
  time += static_cast<float>(deltaTime);

  for (auto &obj : dnaHelix1) {
    obj->rotate(ROTATION_SPEED * deltaTime, mgl::vec3(0.0f, 0.0f, 1.0f));
  }
  for (auto &obj : dnaHelix2) {
    obj->rotate(ROTATION_SPEED * deltaTime, mgl::vec3(1.0f, 0.0f, 0.0f));
  }

  helix->rotate(ROTATION_SPEED * deltaTime, mgl::vec3(0.0f, 0.0f, 1.0f));
}

/////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char *argv[]) {
  mgl::Engine &engine = mgl::Engine::getInstance();
  engine.setApp(std::make_shared<MyApp>());
  engine.setOpenGL(3, 3);
  engine.setWindow(800, 600, "Mesh Loader", 0, 1);
  engine.init();
  engine.run();
  exit(EXIT_SUCCESS);
}

////////////////////////////////////////////////////////////////////////////////
