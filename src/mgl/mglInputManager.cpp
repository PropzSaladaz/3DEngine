#include <mgl/mglInputManager.hpp>
#include <iostream>

namespace mgl {


InputManager::InputManager() {
    mousePos= { 0, 0 };
    mouseMove = { 0, 0 };
}

InputManager::~InputManager() {}

InputManager& InputManager::getInstance() {
    static InputManager instance;
    return instance;
}


////////////////////////////////////////////////////////////////////////// GLFW STATIC CALLBACKS

// GLFW static function callbacks for handling window/mouse/keyboard input
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    InputManager::getInstance().keyCallback(window, key, scancode, action, mods);
}

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
    InputManager::getInstance().cursorPosCallback(window, xpos, ypos);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    InputManager::getInstance().mouseButtonCallback(window, button, action, mods);
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    InputManager::getInstance().scrollCallback(window, xoffset, yoffset);
}

static void window_size_callback(GLFWwindow* window, int width, int height) {
    InputManager::getInstance().windowSizeCallback(window, width, height);
}

static void joystick_callback(int jid, int event) {}

/// <summary>
/// Initializes all input callbacks within GLFW
/// Must be called at the start of the engine to capture all input events
/// </summary>
/// <param name="window"> GLFW window which fires the events </param>
void InputManager::setupCallbacks(GLFWwindow* window) {
    // cursor position
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    // keyboard
    glfwSetKeyCallback(window, key_callback);
    // mouse button
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // mouse scroll
    glfwSetScrollCallback(window, scroll_callback);
    // joystick (not used)
    glfwSetJoystickCallback(joystick_callback);
    // window resizing
    glfwSetWindowSizeCallback(window, window_size_callback);
}

////////////////////////////////////////////////////////////////////////// INPUT MANAGER HANDLERS
/// <summary>
/// Updates key pressed status. If key wasn't pressed -> call the associated callbakcs for
/// the pressed key only for the first frame in which the key is pressed.
/// 
/// If key is released, set key status to released
/// </summary>
/// <param name="window"> GLFW window (not used) </param>
/// <param name="key"> key code </param>
/// <param name="scancode"> ... </param>
/// <param name="action"> specify key action type - Press/Release </param>
/// <param name="mods"> ... </param>
void InputManager::keyCallback(GLFWwindow* window, int key, int scancode,
    int action, int mods) {
    if (action == GLFW_PRESS) {
        if (!KEY_PRESSED[key]) {
            KEY_PRESSED[key] = true;
            for (const auto& callback : KEY_CALLBACKS[key]) {
                callback();
            }
        }
    }
    else if (action == GLFW_RELEASE) {
        KEY_PRESSED[key] = false;
    }
}

void InputManager::mouseButtonCallback(GLFWwindow* window, int button,
    int action, int mods) {
    if (action == GLFW_PRESS) {
        if (!MOUSE_PRESSED[button]) {
            MOUSE_PRESSED[button] = true;
            for (const auto& callback : MOUSE_BTN_CALLBACKS[button]) {
                callback();
            }
        }
    }
    else if (action == GLFW_RELEASE) {
        MOUSE_PRESSED[button] = false;
    }
}
void InputManager::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    mouseMove.xOffset = xpos - mousePos.x;
    mouseMove.yOffset = ypos - mousePos.y;
    mousePos.x = xpos;
    mousePos.y = ypos;
    for (const auto& callback : MOUSE_MOVE_CALLBACKS) {
        callback(mouseMove.xOffset, mouseMove.yOffset);
    }
}

void InputManager::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    for (const auto& callback : MOUSE_SCROLL_CALLBACKS) {
        callback(xoffset, yoffset);
    }
}

void InputManager::windowSizeCallback(GLFWwindow* window, int width, int height) {
    for (const auto& callback : WINDOW_SIZE_CALLBACKS) {
        callback(width, height);
    }
}


///////////////////////////////////////////////////////////////// REGISTER CALLBACKS

void InputManager::registerMouseScrollCallBack(const MouseOffsetCallBack callback) {
    MOUSE_SCROLL_CALLBACKS.push_back(callback);
}

void InputManager::registerMouseOffsetCallBack(const MouseOffsetCallBack callback) {
    MOUSE_MOVE_CALLBACKS.push_back(callback);
}

void InputManager::registerWindowSizeCallback(WindowSizeCallBack callback) {
    WINDOW_SIZE_CALLBACKS.push_back(callback);
}

void InputManager::registerKeyCallBack(GLuint key, const KeyCallBack callback) {
    if (key >= GLFW_KEY_LAST) {
        std::cerr << "INPUT_MANAGER::REGISTER_KEY_CALLBACK" << std::endl;
        std::cerr << "Key is out of bounds" << std::endl;
        exit(EXIT_FAILURE);
    }
    KEY_CALLBACKS[key].push_back(callback);
}

void InputManager::registerMouseBtnCallBack(GLuint button, 
                                const KeyCallBack callBack) {
    if (button >= GLFW_MOUSE_BUTTON_LAST) {
        std::cerr << "INPUT_MANAGER::REGISTER_MOUSE_BTN_CALLBACK" << std::endl;
        std::cerr << "Button is out of bounds" << std::endl;
        exit(EXIT_FAILURE);
    }
    MOUSE_BTN_CALLBACKS[button].push_back(callBack);
}

////////////////////////////////////////////////////////////////////////// GETTERS
bool InputManager::isKeyPressed(int key) {
    if (key >= GLFW_KEY_LAST) return false;
    return KEY_PRESSED[key];
}

bool InputManager::isMouseBtnPressed(int button) {
    if (button >= GLFW_MOUSE_BUTTON_LAST) return false;
    return MOUSE_PRESSED[button];
}

MousePos InputManager::getMousePos() {
    return mousePos;
}

/// <summary>
/// Returns mouse displacement in current frame
/// </summary>
/// <returns></returns>
MouseMove InputManager::getMouseMovement() {
    return mouseMove;
}

}