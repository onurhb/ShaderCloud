// ---------------------- INCLUDES
#include "UserInterface.h"
#include <iostream>

UserInterface::UserInterface()
        : io(ImGui::GetIO()) {
    std::fill(soundTrackUrl.begin(), soundTrackUrl.end(), 0);
}


UserInterface::~UserInterface() {
    // - ImGui
    ImGui_ImplGlfwGL3_Shutdown();
}


/**
 * Initialize the interface
 * @param windowPointer : the GLFW window to display on.
 */
void UserInterface::initialize(GLFWwindow *windowPointer) {
    if (windowPointer == nullptr) {
        std::cout << "No window is created. Cannot initialize user interface.";
        return;
    }
    io.Fonts->AddFontFromFileTTF("Ubuntu-R.ttf", 13.0f);
    ImGui_ImplGlfwGL3_Init(windowPointer, true);
}

/**
 * Called before rendering anything
 */
void UserInterface::preRender() {
    ImGui_ImplGlfwGL3_NewFrame();
}

/**
 * Called right before swapping buffers
 */
void UserInterface::preUpdate() {
    ImGui::Render();
}


// ---------------------------------------------------------- COMPONENTS

void UserInterface::renderPlayback(Playback &playback, bool &openFileDialog) {

    if (!displayPlayback) return;
    if (ImGui::Begin("Playback", &displayPlayback, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Playback Settings");
        ImGui::Separator();
        // - Open button
        if (ImGui::Button("Open ...")) openFileDialog = true;

        // - Playback controllers
        PLAYBACK_STATUS status = playback.getStatus();
        if (status == PLAYBACK_PLAYING) {
            ImGui::SameLine();
            // - Pause button
            if (ImGui::Button("Pause")) {
                playback.pauseStream();
            }
        } else if(status == PLAYBACK_PAUSED) {
            ImGui::SameLine();
            // - Play/Resume
            if (ImGui::Button("Play ")) {
                playback.playStream();
            }
        }

        // - Volume
        ImGui::Separator();
        float volume = playback.getVolume();
        if (ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0))playback.setVolume(volume);
        // - Seek
        float playedTime = playback.getPlayedTime();
        float playbackTime = playback.getPlaybackTime();
        if (ImGui::SliderFloat("##", &playedTime, 0, playbackTime, "%.0f s")) playback.seekStreamSeconds(playedTime);
    }
    ImGui::End();
}

/**
 * Render a file modal. Includes filedialog- and SoundCloud input.
 * @param openFileModal : determines if it should visible or not
 * @return : empty if no file has been selected, else is a path or url.
 */
INPUT_OPTIONS UserInterface::renderFileModal(bool &openFileModal) {

    INPUT_OPTIONS option = NO_INPUT;

    ImGui::OpenPopup("Open File");
    if (ImGui::BeginPopupModal("Open File", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {
        ImGui::Text("Please select a source.\n");
        // - Open from local files. Blocks rendering.
        if (ImGui::Button("Local ...", ImVec2(120, 0))) {
            openFileModal = false;
            displaySoundInput = false;
            option = LOCAL_INPUT;
        }
        // - Open from SoundCloud
        ImGui::SameLine();
        if (ImGui::Button("SoundCloud ...", ImVec2(120, 0))) displaySoundInput ^= 1;
        // - Cancel
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            openFileModal = false;
            displaySoundInput = false;
        }
        ImGui::Separator();
        // - Should display input
        if (displaySoundInput) {
            ImGui::Text("SoundCloud Track Link:");
            ImGui::PushItemWidth(-1);
            ImGui::InputText("##", soundTrackUrl.data(), 1024);
            // - Get from SoundCloud
            ImGui::PushItemWidth(-1);
            if (ImGui::Button("Grab", ImVec2(120, 0))) {
                input = soundTrackUrl.data();
                openFileModal = false;
                option = SOUNDCLOUD_INPUT;
            }
        }
        ImGui::EndPopup();
    }

    return option;
}


/**
 * Renders a notification overlay.
 * @param status : a struct containing what to render and how long it should be visible.
 */
void UserInterface::renderNotification() {

    // - Should it render...?
    if (notification.time > 0.0f) (notification.time -= io.DeltaTime) > 0.0f ?: (notification.time = 0.0f);
    else if (notification.time != -1.0f) return;

    ImGui::SetNextWindowPos(ImVec2(12.5f, 12.5f));
    ImGui::Begin("Notification", NULL, ImVec2(0, 0), 0.65f,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Text(notification.message.c_str());
    ImGui::End();

}

/**
 * Used to get inputs from the user interface
 * @return
 */
std::string UserInterface::getInput() const {
    return input;
}


/**
 * Set application status. Used by the interface to show a message.
 * If closeTime is -1 then the notification will not be closed until another one is set.
 * @param status        : message
 * @param closeTime     : time until dissappear
 */
void UserInterface::setNotification(std::string status, float closeTime) {
    this->notification.message = status;
    this->notification.time = closeTime;
}
