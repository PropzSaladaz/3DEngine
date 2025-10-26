#ifndef MGL_APP_HPP
#define MGL_APP_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

namespace mgl {

class App;
class Engine;

//////////////////////////////////////////////////////////////////////////// App

class App {
 public:
  virtual void initCallback(GLFWwindow *window) {}
  virtual void displayCallback(GLFWwindow *window, double elapsed) {}
  virtual void windowCloseCallback(GLFWwindow *window) {}
  virtual void windowSizeCallback(GLFWwindow *window, int width, int height) {}
};

///////////////////////////////////////////////////////////////////////// Engine

class Engine {
 public:
  uint16_t WindowWidth, WindowHeight;

  static Engine &getInstance();

  void setApp(App *app);
  App *getApp();
  void setOpenGL(int major, int minor);
  void setWindow(int width, int height, const char *title, int fullscreen,
                 int vsync);
  void init();
  void run();

 protected:
  virtual ~Engine();

 private:
  Engine();
  App *GlApp;
  int GlMajor, GlMinor;
  GLFWwindow *Window;
  const char *WindowTitle;
  int Fullscreen;
  int Vsync;

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

 public:
  Engine(Engine const &) = delete;
  void operator=(Engine const &) = delete;
};

////////////////////////////////////////////////////////////////////////////////
}  // namespace mgl

#endif /* MGL_APP_HPP */
