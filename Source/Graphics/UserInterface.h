
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
#include <array>

enum UI_EVENTS {
    UI_OPEN_LOCAL_FILE, UI_OPEN_EXTERNAL_FILE, UI_NO_EVENT
};

class UserInterface {

    // - NanoVG
    NVGcontext *ctx = nullptr;
    int font;

    // - Window
    GLFWwindow *window;

    // - ImGui
    ImGuiIO &io;

    // - Settings
    bool displayFileModal = false;
    bool displaySoundCloudModal = false;

    // - Inputs
    std::array<char, 1024> soundCloudInput;

public:
    UserInterface(GLFWwindow *window);
    ~UserInterface();
    void postRender();
    void preRender();
    std::string getInput();
    // - Components
    UI_EVENTS renderLeftPanel();
    UI_EVENTS renderFileModal();
    UI_EVENTS renderNotification(std::string notification);
};


#endif //PROJECT_USERINTERFACE_H
