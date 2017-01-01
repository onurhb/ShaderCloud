#ifndef PROJECT_WINDOW_H
#define PROJECT_WINDOW_H

// ------------------- MACROS
#define MAX_KEYS 1024
// ------------------- INCLUDES
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "../config.h"



class Window {

private:
    std::string windowTitle;
    GLFWwindow *windowPointer;

    int windowWidth, windowHeight;
    bool keys[MAX_KEYS];         // - Keys
    double mouseX, mouseY;      // - Mouse


    double xoffset, yoffset;    // - Scroll
public:
    Window(std::string title, int height, int width);
    ~Window();
    // - Initialize window
    bool initialize();
    // - Handle window
    void update() const;
    bool closed() const;
    void destroy() const;
    void clear();
    // - Getters
    void getMousePosition(double &x, double &y) const;
    void getScrollOffset(double &xoffset, double &yoffset);
    bool getKeyPressed(int key) const;

    GLFWwindow* getWindow() const;
private:
    static bool loadContext();
    friend void keyCallback(GLFWwindow *window_ptr, int key, int scancode, int state, int mods);
    friend void mouseCallback(GLFWwindow *window_ptr, double xpos, double ypos);
    friend void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
};

#endif // PROJECT_WINDOW_H
