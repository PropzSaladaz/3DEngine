#include "mgl/mglApp.hpp"
#include <mgl/mglSimulation.hpp>
#include "mgl/mglError.hpp"
#include <mgl/mglInputManager.hpp>
#include <utils/Logger.hpp>

#include <iostream>

namespace mgl {

/////////////////////////////////////////////////////////////// STATIC CALLBACKS

static void window_close_callback(GLFWwindow *window) {
    Engine::getInstance().getApp().windowCloseCallback(window);
}

static void window_resize_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

static void glfw_error_callback(int error, const char *description) {
    std::cerr << "GLFW Error: " << description << std::endl;
}

////////////////////////////////////////////////////////////////////////// SETUP

Engine::Engine(void) {
    Window = 0;
    WindowWidth = 640, WindowHeight = 480;
    GlMajor = 3, GlMinor = 3;
    Fullscreen = 0, Vsync = 0;
    WindowTitle = "OpenGL App";
}

Engine::~Engine(void) {}

Engine &Engine::getInstance(void) {
  static Engine instance;
  return instance;
}

App& Engine::getApp(void) { return app; }

void Engine::setApp(App app) { this->app = std::move(app); }

void Engine::setOpenGL(int major, int minor) {
  GlMajor = major;
  GlMinor = minor;
}

void Engine::setWindow(int width, int height, const char *title, int fullscreen,
                       int vsync) {
  WindowWidth = width;
  WindowHeight = height;
  WindowTitle = title;
  Fullscreen = fullscreen;
  Vsync = vsync;
}

/////////////////////////////////////////////////////////////////////////// INIT


void Engine::setupGLFW() {
    // set callback for errors
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    setupGLFWWindowHints();
    setupGLFWWindow();
    setupGLFWWindowCallbacks();
}

void Engine::setupGLFWWindowHints() {
    // glfwWindowHints must be specified before window creation
    // specify OpenGL version to use
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GlMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GlMinor);
#ifdef DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
}

void Engine::setupGLFWWindow() {
    // Create Window
    GLFWmonitor* monitor = Fullscreen ? glfwGetPrimaryMonitor() : 0;
    Window = glfwCreateWindow(WindowWidth, WindowHeight, WindowTitle, monitor, 0);
    if (!Window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // configure window
    glfwMakeContextCurrent(Window);
    //glfwSwapInterval(Vsync);
    glfwSwapInterval(0);
}

void Engine::setupGLFWWindowCallbacks() {
    // Attach input callbacks to window
    InputManager::getInstance().setupCallbacks(Window);

    // set global callbacks
    glfwSetWindowCloseCallback(Window, window_close_callback);
    glfwSetFramebufferSizeCallback(Window, window_resize_callback);
}

void Engine::setupGLAD() {
    // GLAD manages function pointers for OpenGL so we
    // want to initialize GLAD before we call any OpenGL function
    // We pass GLAD the function to load the address of the OpenGL function pointers which is
    // OS - specific.GLFW gives us glfwGetProcAddress that defines the correct function based on
    // which OS we�re compiling for.
    if (!gladLoadGL() || !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		exit(EXIT_FAILURE);
	}
    glGetError();
    // GLenum err_code = glGetError(); // Full syntax to check
    // You might get GL_INVALID_ENUM when loading GLEW.
}

void Engine::setupOpenGL() {
  glClearColor(0.05, 0.05, 0.05, 0);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glDepthMask(GL_TRUE);
  glDepthRange(0.0, 1.0);
  glClearDepth(1.0);

  // Enable culling
  glEnable(GL_CULL_FACE);
  // Discard back faces by default
  glCullFace(GL_BACK);
  // front faces are by default following CCW winding
  glFrontFace(GL_CCW);
  // The first two parameters of glViewport set the location of the lower left corner of the window
  glViewport(0, 0, WindowWidth, WindowHeight);
}

void displayInfo() {
    std::cerr << "OpenGL Renderer: " << glGetString(GL_RENDERER) << " ("
            << glGetString(GL_VENDOR) << ")" << std::endl;
    std::cerr << "OpenGL version " << glGetString(GL_VERSION) << std::endl;
    std::cerr << "GLSL version " << glGetString(GL_SHADING_LANGUAGE_VERSION)
            << std::endl;
    std::cout << "GLFW version " << glfwGetVersionString() << std::endl;
    std::cout << GLM_VERSION_MESSAGE << std::endl;
}

void Engine::init() {
    mgl::log::init();
    setupGLFW();
    setupGLAD();
    setupOpenGL();
#ifdef DEBUG
    displayInfo();
    setupDebugOutput();
#endif
}

ResourceContext Engine::getManagers() {
    ResourceContext resources {
        Meshes,
        Textures,
        Shaders,
        Materials
    };
    return resources;
}

void Engine::setupManagers() {
    Meshes = std::move(MeshManager());
    Textures = std::move(TextureManager());
    Shaders = std::move(ShaderManager());
    Materials = std::move(MaterialManager());
}

Scene& Engine::createMainScene() {
    Scene& scene = *(new Scene(&Meshes, &Shaders, &Textures));
    return scene;
}

//////////////////////////////////////////////////////////////////////////// RUN

void Engine::run() {
    // setup
    setupManagers();
    ResourceContext resources = getManagers();
    app.onRegisterGlobalResources(resources);

    Scene& scene = createMainScene();
    app.onCreateScene(scene, resources);
    app.onStart();

    // start running loop
    double last_time = glfwGetTime();

    // wait for window to be closed
    while (!glfwWindowShouldClose(Window)) { // render loop

        // update deltaTime
        double time = glfwGetTime();
        double elapsed_time = time - last_time;
        last_time = time;
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glfwSetWindowTitle(Window, std::to_string(static_cast<int>(1.0 / elapsed_time)).c_str());
        Simulation::getInstance().update(elapsed_time);

        // Let App render / display
        app.onUpdate( elapsed_time);

        // Swap buffers and poll events
        glfwSwapBuffers(Window);
        glfwPollEvents();
    }
    glfwDestroyWindow(Window);
    glfwTerminate();
}

////////////////////////////////////////////////////////////////////////////////
}  // namespace mgl
