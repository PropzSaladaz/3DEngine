#include "mgl/mglApp.hpp"
#include <mgl/mglSimulation.hpp>
#include "mgl/mglError.hpp"
#include <mgl/mglInputManager.hpp>
#include <utils/logger.hpp>

#include <iostream>

namespace mgl {

/////////////////////////////////////////////////////////////// STATIC CALLBACKS

static void window_close_callback(GLFWwindow *window) {
    Engine::getInstance().getApp()->windowCloseCallback(window);
}

static void window_resize_callback(GLFWwindow* window, int width, int height) {
    Engine::getInstance().getApp()->windowSizeCallback(window, width, height);
}

static void glfw_error_callback(int error, const char *description) {
    std::cerr << "GLFW Error: " << description << std::endl;
}

////////////////////////////////////////////////////////////////////////// SETUP

Engine::Engine(void) {
    GlApp = 0;
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

App *Engine::getApp(void) { return GlApp; }

void Engine::setApp(App *app) { GlApp = app; }

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
    // GLFW specific -------------------------
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    // glfwWindowHints must be specified before window creation
    // specify OpenGL version to use
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GlMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GlMinor);
#ifdef DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

    // Engine ---------------------------------
    setupWindow();
    setupCallbacks();
}

void Engine::setupWindow() {
    GLFWmonitor* monitor = Fullscreen ? glfwGetPrimaryMonitor() : 0;
    Window = glfwCreateWindow(WindowWidth, WindowHeight, WindowTitle, monitor, 0);
    if (!Window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(Window);
    //glfwSwapInterval(Vsync);
    glfwSwapInterval(0);
}

void Engine::setupCallbacks() {
    InputManager::getInstance().setupCallbacks(Window);
    glfwSetWindowCloseCallback(Window, window_close_callback);
    glfwSetFramebufferSizeCallback(Window, window_resize_callback);
}

void Engine::setupGLAD() {
    // GLAD manages function pointers for OpenGL so we
    // want to initialize GLAD before we call any OpenGL function
    // We pass GLAD the function to load the address of the OpenGL function pointers which is
    // OS - specific.GLFW gives us glfwGetProcAddress that defines the correct function based on
    // which OS we’re compiling for.
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
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
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
    setupGLFW();
    setupGLAD();
    setupOpenGL();
    GlApp->initCallback(Window);
#ifdef DEBUG
    util::Logger::SetDebugActive();
    displayInfo();
    setupDebugOutput();
#endif
}

//////////////////////////////////////////////////////////////////////////// RUN

void Engine::run() {
    double last_time = glfwGetTime();
    while (!glfwWindowShouldClose(Window)) { // render loop
        // update deltaTime
        double time = glfwGetTime();
        double elapsed_time = time - last_time;
        last_time = time;
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        //std::cout << "here" << std::endl;
        glfwSetWindowTitle(Window, std::to_string(static_cast<int>(1.0 / elapsed_time)).c_str());
        Simulation::getInstance().update(elapsed_time);
        GlApp->displayCallback(Window, elapsed_time);

        glfwSwapBuffers(Window);
        glfwPollEvents();
    }
    glfwDestroyWindow(Window);
    glfwTerminate();
}

////////////////////////////////////////////////////////////////////////////////
}  // namespace mgl
