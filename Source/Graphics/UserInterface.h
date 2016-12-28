

#ifndef PROJECT_INTERFACE_H
#define PROJECT_INTERFACE_H

// ------------------ INCLUDES
#include <array>
#include <imgui.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui_binding.h"
#include "../Audio/Playback.h"


// - Notification - Used to show a notification
struct NOTIFICATION_STATUS{
    std::string message;
    float time = 0.0f;
};

// - Options - Used to tell if user has requested local or external audio file
enum INPUT_OPTIONS {LOCAL_INPUT, SOUNDCLOUD_INPUT, NO_INPUT};

class UserInterface {

    // - ImGui i/o
    ImGuiIO& io;

    // - Things to display
    bool displayPlayback = true;
    bool displaySoundInput = false;

    // - Last location
    std::array<char, 1024> soundTrackUrl;
    std::string input;

    // - Notification
    NOTIFICATION_STATUS notification;


public:
    UserInterface();
    ~UserInterface();
    // - Should be called after creating the window
    void initialize(GLFWwindow *windowPointer);
    // - Should be called before rendering the window
    void preRender();
    // - Should be called after rendering the window
    void preUpdate();

    // ------------------------------------ COMPONENTS
    void renderPlayback(Playback &playback, bool& openFile);
    INPUT_OPTIONS renderFileModal(bool& openFileModal);
    void renderNotification();

    // ------------------------------------ GETTERS
    std::string getInput() const;
    // ------------------------------------ SETTERS
    void setNotification(std::string status, float closeTime = -1.0f);

};


#endif //PROJECT_INTERFACE_H
