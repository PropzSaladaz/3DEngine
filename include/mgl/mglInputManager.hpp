#ifndef MGL_INPUTMANAGER_HPP
#define MGL_INPUTMANAGER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <functional>
#include <vector>

namespace mgl {

using KeyCallBack = std::function<void()>;
using MouseOffsetCallBack = std::function<void(GLfloat xOffset, GLfloat yOffset)>;
using WindowSizeCallBack = std::function<void(GLuint width, GLuint height)>;

struct MouseMove {
    double xOffset;
    double yOffset;
};

struct MousePos {
    double x;
    double y;
};

class InputManager {
public:
    InputManager();
    InputManager(InputManager const&) = delete;
    virtual ~InputManager();
    void operator=(InputManager const&) = delete;
    static InputManager& getInstance();
    

    // keyboard
    bool isKeyPressed(int key);
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void registerKeyCallBack(GLuint key, const KeyCallBack callBack);

    // mouse
    MousePos getMousePos();
    MouseMove getMouseMovement();
    bool isMouseBtnPressed(int button);
    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    void registerMouseBtnCallBack(GLuint key, const KeyCallBack callback);
    void registerMouseOffsetCallBack(const MouseOffsetCallBack callback);

    // screen
    void windowSizeCallback(GLFWwindow* window, int width, int height);
    void registerWindowSizeCallback(WindowSizeCallBack callback);

    
private:
    // keyboard
    bool KEY_BUFFER[GLFW_KEY_LAST] = { false };
    bool KEY_PRESSED[GLFW_KEY_LAST] = { false };
    std::vector<KeyCallBack> KEY_CALLBACKS[GLFW_KEY_LAST]; // callbacks per key


    // mouse
    MouseMove   mouseMove; // records curent offsets
    MousePos    position;
    bool MOUSE_BUFFER[GLFW_MOUSE_BUTTON_LAST] = { false };
    bool MOUSE_PRESSED[GLFW_MOUSE_BUTTON_LAST] = { false };
    std::vector<KeyCallBack> MOUSE_BTN_CALLBACKS[GLFW_MOUSE_BUTTON_LAST]; // callbacks per mouse btn
    std::vector<MouseOffsetCallBack> MOUSE_MOVE_CALLBACKS; // callbacks per mouse movement

    // window resize
    std::vector<WindowSizeCallBack> WINDOW_SIZE_CALLBACKS;



};

} // namespace mgl

#endif