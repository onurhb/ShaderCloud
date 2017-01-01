
#ifndef PROJECT_USERINTERFACE_H
#define PROJECT_USERINTERFACE_H
// -------------- INCLUDES
#include "../config.h"
#include <nanovg.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_binding.h>


class UserInterface {

    // - Interface
    NVGcontext* ctx = nullptr;

    // - Window
    GLFWwindow* window;

    // - ImGui
    ImGuiIO& io;

private:
    void renderLeftPanel();
    void renderPanelContent();

public:
    UserInterface(GLFWwindow* window);
    ~UserInterface();
    void render(double timestamp);
};


#endif //PROJECT_USERINTERFACE_H
