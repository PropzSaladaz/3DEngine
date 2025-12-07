#include <mgl/mgl.hpp>
#include <memory>
#include <vector>
#include <mgl/mglInputManager.hpp>

////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App {
public:
    void onRegisterGlobalResources(mgl::ResourceContext& resources) override;
    void onCreateScene(mgl::Scene& scene, mgl::ResourceContext& resources) override;

private:
    struct FilterConfig {
        std::string name;
        mgl::TextureFilter minFilter;
        mgl::TextureFilter magFilter;
        float anisotropy;
    };

    struct WrapConfig {
        std::string name;
        mgl::TextureWrap wrapS;
        mgl::TextureWrap wrapT;
    };

    const std::vector<FilterConfig> filterConfigs = {
        { "nearest", mgl::TextureFilter::Nearest, mgl::TextureFilter::Nearest, 0.0f },
        { "linear", mgl::TextureFilter::Linear, mgl::TextureFilter::Linear, 0.0f },
        { "nearest_mipmap_nearest", mgl::TextureFilter::NearestMipmapNearest, mgl::TextureFilter::Linear, 0.0f },
        { "nearest_mipmap_linear", mgl::TextureFilter::NearestMipmapLinear, mgl::TextureFilter::Linear, 0.0f },
        { "linear_mipmap_nearest", mgl::TextureFilter::LinearMipmapNearest, mgl::TextureFilter::Linear, 0.0f },
        { "linear_mipmap_linear", mgl::TextureFilter::LinearMipmapLinear, mgl::TextureFilter::Linear, 0.0f },
        { "anisotropic", mgl::TextureFilter::LinearMipmapLinear, mgl::TextureFilter::Linear, 8.0f },
    };

    const std::vector<WrapConfig> wrapConfigs = {
        { "wrap_repeat", mgl::TextureWrap::Repeat, mgl::TextureWrap::Repeat },
        { "wrap_clamp_edge", mgl::TextureWrap::ClampToEdge, mgl::TextureWrap::ClampToEdge },
        { "wrap_mirrored_repeat", mgl::TextureWrap::MirroredRepeat, mgl::TextureWrap::MirroredRepeat },
        { "wrap_clamp_border", mgl::TextureWrap::ClampToBorder, mgl::TextureWrap::ClampToBorder },
    };

    const float initialScreenRatio = 800.0f / 600.0f;
    const GLuint UBO_BP = 0;
    std::shared_ptr<mgl::FPSCamController> FPSCamera;
    GLuint cam = 0;
    mgl::PerspectiveParams perspectiveP = { 60.0f, 800.0f / 600.0f, 0.1f, 100.0f };
    std::shared_ptr<mgl::Texture2D> checkerTexture;

    // scene
    void createMeshes(mgl::MeshManager& meshManager);
    void createTextures(mgl::TextureManager& textureManager);
    void createMaterials(mgl::MaterialManager& materialManager);
    void createShaderPrograms(mgl::ShaderManager& shaderManager);
};

///////////////////////////////////////////////////////////////////////// MESHES

void MyApp::createMeshes(mgl::MeshManager& meshManager) {
    std::shared_ptr<mgl::Mesh> quad = std::make_shared<mgl::Mesh>();
    const float tileRepeat = 16.0f; // tile heavily to force minification differences
    mgl::MeshData data = {
        .positions = {
            mgl::vec3(-0.5f, -0.5f, 0.0f),
            mgl::vec3( 0.5f, -0.5f, 0.0f),
            mgl::vec3( 0.5f,  0.5f, 0.0f),
            mgl::vec3(-0.5f,  0.5f, 0.0f),
        },
        .indices = { 0, 1, 2, 2, 3, 0 },
        .texcoords = {
            mgl::vec2(0.0f, 0.0f),
            mgl::vec2(tileRepeat, 0.0f),
            mgl::vec2(tileRepeat, tileRepeat),
            mgl::vec2(0.0f, tileRepeat),
        },
    };
    quad->createFromData(data);
    meshManager.add("quad", quad);
}

void MyApp::createTextures(mgl::TextureManager& manager) {
    checkerTexture = std::make_shared<mgl::Texture2D>();
    // High-frequency pattern (1px cells) to exaggerate filtering differences
    checkerTexture->genCheckerboard(64, 64, 64);
    manager.add("checkerboard", checkerTexture);

    std::shared_ptr<mgl::Texture2D> grassTexture = std::make_shared<mgl::Texture2D>();
    grassTexture->load("textures/grass.png");
    manager.add("grass", grassTexture);
    std::shared_ptr<mgl::Texture2D> brickTexture = std::make_shared<mgl::Texture2D>();
    brickTexture->load("textures/wood.png");
    manager.add("wood", brickTexture);
}

///////////////////////////////////////////////////////////////////////// SHADER

void MyApp::createShaderPrograms(mgl::ShaderManager& shaderManager) {
    mgl::ShaderBuilder simpleShaders;
    simpleShaders.addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);
    simpleShaders.addAttribute(mgl::TEXCOORD_ATTRIBUTE, mgl::Mesh::TEXCOORD);
    simpleShaders.addShader(GL_VERTEX_SHADER, "shaders/vert.glsl");
    simpleShaders.addShader(GL_FRAGMENT_SHADER, "shaders/frag.glsl");
    simpleShaders.addUniforms<mgl::BasicMaterial>();
    simpleShaders.addUniform(mgl::MODEL_MATRIX);
    simpleShaders.addUniform("texture1");
    simpleShaders.addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);

    std::shared_ptr<mgl::ShaderProgram> program = std::make_shared<mgl::ShaderProgram>(simpleShaders.build());
    shaderManager.add("simple", program);
}

///////////////////////////////////////////////////////////////////////// SCENE


void MyApp::onCreateScene(mgl::Scene& scene, mgl::ResourceContext& resources) {
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
    FPSCamera->setMovementSpeed(2.5f);
    FPSCamera->setMouseSensitivity(0.1f);
    FPSCamera->setActive();

    auto quadMesh = resources.meshManager.get("quad");
    auto shader = resources.shaderManager.get("simple");

    std::shared_ptr<mgl::SceneGraph> graph = std::make_shared<mgl::SceneGraph>();
    const float spacing = 2.0f; // 1 unit gap between 1x1 quads
    float filterStartX = -static_cast<float>(filterConfigs.size() - 1) * spacing * 0.5f;
    float wrapStartX = -static_cast<float>(wrapConfigs.size() - 1) * spacing * 0.5f;

    // Wrap row (upper)
    for (size_t i = 0; i < wrapConfigs.size(); ++i) {
        const auto& cfg = wrapConfigs[i];
        auto obj = std::make_shared<mgl::SceneObject>(quadMesh);
        obj->setShaders(shader);
        obj->setMaterial(resources.materialManager.get(cfg.name));
        obj->setPosition(wrapStartX + spacing * static_cast<float>(i), 2.5f, 0.f);
        obj->rotate(-60.0f, mgl::vec3(1.0f, 0.0f, 0.0f));
        graph->add(obj);
    }

    // Filter row (lower)
    for (size_t i = 0; i < filterConfigs.size(); ++i) {
        const auto& cfg = filterConfigs[i];
        auto obj = std::make_shared<mgl::SceneObject>(quadMesh);
        obj->setShaders(shader);
        obj->setMaterial(resources.materialManager.get(cfg.name));
        obj->setPosition(filterStartX + spacing * static_cast<float>(i), 0.f, 0.f);
        obj->rotate(-60.0f, mgl::vec3(1.0f, 0.0f, 0.0f)); // tilt to highlight anisotropy
        graph->add(obj);
    }

    scene.setScenegraph(graph);
}

void MyApp::createMaterials(mgl::MaterialManager& materialManager) {
    // Wrap materials (use linear filtering for clarity)
    for (const auto& cfg : wrapConfigs) {
        auto sampler = std::make_shared<mgl::Sampler>();
        sampler->create(mgl::TextureFilter::Linear, mgl::TextureFilter::Linear);
        sampler->setWrap(cfg.wrapS, cfg.wrapT);
        auto texInfo = std::make_shared<mgl::TextureSampler>(GL_TEXTURE0, 0, "texture1", checkerTexture, sampler);
        auto material = std::make_shared<mgl::BasicMaterial>(mgl::vec4(1.0f));
        material->addTexture(texInfo);
        materialManager.add(cfg.name, material);
    }

    for (const auto& cfg : filterConfigs) {
        auto sampler = std::make_shared<mgl::Sampler>();
        sampler->create(cfg.minFilter, cfg.magFilter);
        if (cfg.anisotropy > 0.0f) {
            sampler->setAnisotropy(cfg.anisotropy);
        }
        auto texInfo = std::make_shared<mgl::TextureSampler>(GL_TEXTURE0, 0, "texture1", checkerTexture, sampler);
        auto material = std::make_shared<mgl::BasicMaterial>(mgl::vec4(1.0f));
        material->addTexture(texInfo);
        materialManager.add(cfg.name, material);
    }
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::onRegisterGlobalResources(mgl::ResourceContext& resources) {
    createMeshes(resources.meshManager);
    createTextures(resources.textureManager);
    createMaterials(resources.materialManager);
    createShaderPrograms(resources.shaderManager);  // after mesh;

    mgl::InputManager::getInstance().registerKeyCallBack(GLFW_KEY_TAB, [this]() {
        mgl::Engine::getInstance().toggleCursorCapture();
    });

    // press 1 to 3 to change textures on all materials
    mgl::InputManager::getInstance().registerKeyCallBack(GLFW_KEY_1, [this]() {
        for (const auto& name : wrapConfigs) {
            auto mat = mgl::Engine::getInstance().getMaterialManager().get(name.name);
            if (mat) {
                mat->setTexture("texture1", mgl::Engine::getInstance().getTextureManager().get("checkerboard"));
            }
        }

        for (const auto& name : filterConfigs) {
            auto mat = mgl::Engine::getInstance().getMaterialManager().get(name.name);
            if (mat) {
                mat->setTexture("texture1", mgl::Engine::getInstance().getTextureManager().get("checkerboard"));
            }
        }
    });

    mgl::InputManager::getInstance().registerKeyCallBack(GLFW_KEY_2, [this]() {
        for (const auto& name : wrapConfigs) {
            auto mat = mgl::Engine::getInstance().getMaterialManager().get(name.name);
            if (mat) {
                mat->setTexture("texture1", mgl::Engine::getInstance().getTextureManager().get("grass"));
            }
        }

        for (const auto& name : filterConfigs) {
            auto mat = mgl::Engine::getInstance().getMaterialManager().get(name.name);
            if (mat) {
                mat->setTexture("texture1", mgl::Engine::getInstance().getTextureManager().get("grass"));
            }
        }
    });

    mgl::InputManager::getInstance().registerKeyCallBack(GLFW_KEY_3, [this]() {
        for (const auto& name : wrapConfigs) {
            auto mat = mgl::Engine::getInstance().getMaterialManager().get(name.name);
            if (mat) {
                mat->setTexture("texture1", mgl::Engine::getInstance().getTextureManager().get("wood"));
            }
        }

        for (const auto& name : filterConfigs) {
            auto mat = mgl::Engine::getInstance().getMaterialManager().get(name.name);
            if (mat) {
                mat->setTexture("texture1", mgl::Engine::getInstance().getTextureManager().get("wood"));
            }
        }
    });
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
