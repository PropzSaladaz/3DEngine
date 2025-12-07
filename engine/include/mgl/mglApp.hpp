#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "mgl/mglShaderManager.hpp"
#include "mgl/models//textures/mglTextureManager.hpp"
#include "mgl/models/meshes/mglMeshManager.hpp"
#include "mgl/models/materials/mglMaterialManager.hpp"
#include "mgl/scene/mglSceneGraph.hpp"

namespace mgl {

class App;
class Engine;

struct ResourceContext {
    MeshManager&     meshManager;
    TextureManager&  textureManager;
    ShaderManager&   shaderManager;
    MaterialManager& materialManager;
};

class App {
public:
    virtual void onRegisterGlobalResources(ResourceContext& resources) {};
    virtual void onCreateScene(Scene& scene, ResourceContext& resources) {};
    virtual void onStart() {};
    virtual void onUpdate(double deltaTime) {};
    virtual ~App() = default;
};


class Engine {
public:
    static Engine &getInstance();

    void setApp(std::shared_ptr<App> app);
    App& getApp();
    void setOpenGL(int major, int minor);
    void setWindow(int width, int height, const char *title, int fullscreen,
                  int vsync);
    void setCursorCapture(bool enabled);
    void toggleCursorCapture();
    
    void init();
    void run();

    MeshManager&     getMeshManager()     { return *Meshes; }
    TextureManager&  getTextureManager()  { return *Textures; }
    ShaderManager&   getShaderManager()   { return *Shaders; }
    MaterialManager& getMaterialManager() { return *Materials; }

    Engine(Engine const &) = delete;
    void operator=(Engine const &) = delete;

protected:
    virtual ~Engine();

private:
    std::shared_ptr<App> app;
    int GlMajor, GlMinor;
    uint16_t WindowWidth, WindowHeight;

    // Window
    GLFWwindow *Window;
    const char *WindowTitle;
    int Fullscreen;
    int Vsync;
    bool CursorCaptured = false;

    // managers - TODO check if shared_ptr is needed
    std::shared_ptr<MeshManager> Meshes;
    std::shared_ptr<TextureManager> Textures;
    std::shared_ptr<ShaderManager> Shaders;
    std::shared_ptr<MaterialManager> Materials;
    Engine();


    void setupWindow();

    /**
     * @brief Initializes GLFW library and sets up OpenGL context configuration.
     * 
     * This function performs the essential GLFW initialization steps including:
     * - Setting up error callback handling
     * - Initializing the GLFW library
     * - Configuring OpenGL version hints using GlMajor and GlMinor
     * - Enabling debug context in DEBUG builds
     * - Calling setupWindow() and setupCallbacks() to complete the setup
     * 
     * The function will terminate the program if GLFW initialization fails.
     * 
     * @note This function must be called before any other GLFW operations.
     * @note Window hints are applied to subsequently created windows.
     * 
     * @throws Exits with EXIT_FAILURE if GLFW initialization fails.
     */
    void setupGLFW();
    void setupGLFWWindow();
    void setupGLFWWindowHints();
    void setupGLFWWindowCallbacks();

    void setupGLAD();
    void setupOpenGL();
    void setupCallbacks();


    void setupManagers();
    ResourceContext getManagers();
    std::shared_ptr<Scene> createMainScene();
};

}  // namespace mgl
