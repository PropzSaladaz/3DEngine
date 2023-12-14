#include <mgl/mglInputManager.hpp>
#include <iostream>

namespace mgl {


InputManager::InputManager() {
    position = { 0, 0 };
    mouseMove = { 0, 0 };
}

InputManager::~InputManager() {}

InputManager& InputManager::getInstance() {
    static InputManager instance;
    return instance;
}


////////////////////////////////////////////////////////////////////////// GLFW CALLBACKS
void InputManager::keyCallback(GLFWwindow* window, int key, int scancode,
    int action, int mods) {
    if (action == GLFW_PRESS) {
        if (!KEY_PRESSED[key]) {
            KEY_PRESSED[key] = true;
            for (const auto& callback : KEY_CALLBACKS[key]) {
                callback();
            }
        }
        KEY_BUFFER[key] = true;
    }
    else if (action == GLFW_RELEASE) {
        KEY_BUFFER[key] = false;
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
        MOUSE_BUFFER[button] = true;
    }
    else if (action == GLFW_RELEASE) {
        MOUSE_BUFFER[button] = false;
        MOUSE_PRESSED[button] = false;
    }
}
void InputManager::cursorPosCallback(GLFWwindow* window, double xpos,
    double ypos) {
    mouseMove.xOffset = xpos - position.x;
    mouseMove.yOffset = ypos - position.y;
    position.x = xpos;
    position.y = ypos;
    for (const auto& callback : MOUSE_MOVE_CALLBACKS) {
        callback(mouseMove.xOffset, mouseMove.yOffset);
    }
}

void InputManager::windowSizeCallback(GLFWwindow* window, int width, int height) {
    for (const auto& callback : WINDOW_SIZE_CALLBACKS) {
        callback(width, height);
    }
}
///////////////////////////////////////////////////////////////// REGISTER CALLBACKS
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
    std::cout << "registered new callback: " << KEY_CALLBACKS[key].size() << std::endl;
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
    return KEY_BUFFER[key];
}

bool InputManager::isMouseBtnPressed(int button) {
    if (button >= GLFW_MOUSE_BUTTON_LAST) return false;
    return MOUSE_BUFFER[button];
}

MousePos InputManager::getMousePos() {
    return position;
}

MouseMove InputManager::getMouseMovement() {
    return mouseMove;
}

}