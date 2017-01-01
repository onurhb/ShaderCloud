
// ------------------- INCLUDES
#include <iostream>
#include "Window.h"


Window::Window(std::string title, int width, int height) {
    this->windowTitle = title;
    this->windowWidth = width;
    this->windowHeight = height;
    // - Initialize buffers
    for (int i = 0; i < MAX_KEYS; ++i) keys[i] = false;

    initialize();
}


Window::~Window() {
    // - Window
    glfwTerminate();
    glfwDestroyWindow(windowPointer);
}

/**
* Friendly callback function used by GLFW to call when a key event happens
* @param window: pointer to the window which GLFW received event from
* @param key : key-code pressed
* @param state: pressed, released
* @param scancode: a code representing the key
*/
void keyCallback(GLFWwindow *window_ptr, int key, int scancode, int state, int mods) {
    // - Action tells if key is pressed or released
    Window *_window = static_cast<Window *>(glfwGetWindowUserPointer(window_ptr));
    _window->keys[key] = state != GLFW_RELEASE;
}

/**
  * Friendly callback function used by GLFW to call when a mouse event happens
  * @param window : pointer to window where event occurred
  * @param mx : mouse x cameraPosition
  * @param my : mouse y cameraPosition
  */
void mouseCallback(GLFWwindow *window_ptr, double mx, double my) {
    Window *_window = static_cast<Window *>(glfwGetWindowUserPointer(window_ptr));
    _window->mouseX = mx;
    _window->mouseY = my;
}
/**
 * Scroll
 * @param window_ptr
 * @param xoffset
 * @param yoffset
 */
void scrollCallback(GLFWwindow *window_ptr, double xoffset, double yoffset) {
    Window *_window = static_cast<Window *>(glfwGetWindowUserPointer(window_ptr));
    _window->xoffset = xoffset;
    _window->yoffset = yoffset;
}
/**
 * Resize
 * @param window_ptr
 * @param width
 * @param height
 */
void frameBufferCallback(GLFWwindow *window_ptr, int width, int height) {
    Window *_window = static_cast<Window *>(glfwGetWindowUserPointer(window_ptr));
    glViewport(0, 0, width, height);
}

/**
 * Prepare window
 * @return true/false
 */
bool Window::initialize() {
    // - Clear GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    // - Create a window and its OpenGL context
    windowPointer = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), nullptr, nullptr);

    if (!windowPointer) {
        std::cerr << "Failed to create a window with GLFW" << std::endl;
        glfwTerminate();
        return false;
    }

    // - Use window as current context
    glfwMakeContextCurrent(windowPointer);

    loadContext();

    // - Callbacks
    glfwSetWindowUserPointer(windowPointer, this);
    glfwSetKeyCallback(windowPointer, keyCallback);
    glfwSetScrollCallback(windowPointer, scrollCallback);
    glfwSetCursorPosCallback(windowPointer, mouseCallback);
    glfwSetFramebufferSizeCallback(windowPointer, frameBufferCallback);

    return true;
}

/**
 * Load glad, OPENGL functions.
 * @return
 */
bool Window::loadContext() {
    if (!gladLoadGL()) {
        std::cerr << "Failed to load GLAD" << std::endl;
        return false;
    }
    return true;
}

/**
 * Checks if window is alive
 * @return true/false
 */
bool Window::closed() const {
    return (bool) glfwWindowShouldClose(windowPointer);
}

/**
 * Closes window
 */
void Window::destroy() const {
    glfwSetWindowShouldClose(windowPointer, 0);
}

/**
 * Returns mouse position
 * @param x
 * @param y
 */
void Window::getMousePosition(double &x, double &y) const {
    x = mouseX;
    y = mouseY;
}

/**
 * Returns the key pressed
 * @param key
 * @return true/false
 */
bool Window::getKeyPressed(int key) const {
    return keys[key];
}

/**
 * Clear frame buffer
 */
void Window::clear() {
    // - Clears screen
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

}

/**
 * Update window and swap buffers
 */
void Window::update() const {
    glfwPollEvents();
#if WINDOW_VSYNC
    glfwSwapInterval(1);
#endif
    glfwSwapBuffers(windowPointer);
}

/**
 * Returns scroll offsett
 * @param xoffset
 * @param yoffset
 */
void Window::getScrollOffset(double &xoffset, double &yoffset) {
    xoffset = this->xoffset;
    yoffset = this->yoffset;
}


/**
 * Returns a pointer to the window
 * @return GLFWwindow*
 */
GLFWwindow* Window::getWindow() const {
    return windowPointer;
}



