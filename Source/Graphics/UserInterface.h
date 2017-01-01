
#ifndef PROJECT_USERINTERFACE_H
#define PROJECT_USERINTERFACE_H
// -------------- INCLUDES
#include "../config.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_binding.h>
#include <nanovg.h>
#include <string>

enum UI_EVENTS {UI_OPEN_LOCAL_FILE, UI_NO_EVENT};

class UserInterface {

    // - NanoVG
    NVGcontext* ctx = nullptr;
    int font = 0;

    // - Window
    GLFWwindow* window;

    // - ImGui
    ImGuiIO& io;

    // - Settings
    bool displayFileModal = false;

public:
    UserInterface(GLFWwindow* window);
    ~UserInterface();
    void postRender();
    void preRender();

    // - Components
    UI_EVENTS renderLeftPanel();
    UI_EVENTS renderFileModal();
    UI_EVENTS renderNotification(std::string notification);
};


#endif //PROJECT_USERINTERFACE_H
