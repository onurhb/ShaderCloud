#define NANOVG_GL3_IMPLEMENTATION
// -------------- INCLUDES
#include "UserInterface.h"
#include "nanovg_gl.h"

UserInterface::UserInterface(GLFWwindow *window)
        : io(ImGui::GetIO()) {
    // - Window
    this->window = window;

    // - NanoVG
    ctx = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
    font = nvgCreateFont(ctx, "system", "Ubuntu-M.ttf");

    // - ImGui
    ImGui_ImplGlfwGL3_Init(window, true);
    io.Fonts->AddFontFromFileTTF("Ubuntu-M.ttf", 13.0f);
}

UserInterface::~UserInterface() {
    ImGui_ImplGlfwGL3_Shutdown();
}

void UserInterface::preRender() {
    ImGui_ImplGlfwGL3_NewFrame();
    nvgBeginFrame(ctx, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_WIDTH / WINDOW_HEIGHT);
}

void UserInterface::postRender() {
    nvgEndFrame(ctx);
    ImGui::Render();
}
// -------------------------------------------------------------------------- COMPONENTS

UI_EVENTS UserInterface::renderLeftPanel() {

    UI_EVENTS events = UI_NO_EVENT;

    // - Menu
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open ...", NULL)) {
                displayFileModal = true;
            }
            ImGui::Separator();
            ImGui::MenuItem("Save", NULL);
            ImGui::MenuItem("Undo", NULL);
            ImGui::MenuItem("Redo", NULL);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Settings")) {
            if (ImGui::MenuItem("Hide widgets", NULL));
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help")) {
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    return events;
}

UI_EVENTS UserInterface::renderFileModal() {
    UI_EVENTS events = UI_NO_EVENT;
    if (!displayFileModal) return events;

    ImGui::OpenPopup("Open File");
    if (ImGui::BeginPopupModal("Open File", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {
        ImGui::Text("Please select a source.\n");
        if (ImGui::Button("Local Track...", ImVec2(120, 0))) {
            displayFileModal = false;
            events = UI_OPEN_LOCAL_FILE;
        }
        ImGui::SameLine();
        if (ImGui::Button("Local Shader...", ImVec2(120, 0))) {
            displayFileModal = false;
            events = UI_OPEN_SHADER;
        }
        ImGui::SameLine();
        if (ImGui::Button("Soundcloud Track...", ImVec2(120, 0))){
            displaySoundCloudModal ^= 1;
        }
        // - Cancel
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            displayFileModal = false;
        }

        if (displaySoundCloudModal){
            ImGui::Separator();
            ImGui::Text("Track Link:");
            ImGui::PushItemWidth(-1);
            ImGui::InputText("##", soundCloudInput.data(), soundCloudInput.size());
            if(ImGui::Button("Download", ImVec2(160, 0))){
                displaySoundCloudModal = displayFileModal = false;
                events = UI_OPEN_EXTERNAL_FILE;
            }
        }

        ImGui::EndPopup();
    }

    return events;
}

UI_EVENTS UserInterface::renderNotification(std::string notification) {

    if(notification.empty()) return UI_NO_EVENT;

    int width = 300, height = 50;
    int posX = WINDOW_WIDTH - width - 10;
    int posY =  WINDOW_HEIGHT - height - 10;

    nvgBeginPath(ctx);
    nvgRoundedRect(ctx, posX, posY, width, height, 2);
    nvgFillColor(ctx, nvgRGBA(35, 32, 40, 200));
    nvgFill(ctx);

    nvgBeginPath(ctx);
    nvgRoundedRect(ctx, posX, posY, width, height, 2);
    nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 255));
    nvgStrokeWidth(ctx, 3);
    nvgStroke(ctx);

    nvgFontSize(ctx, 14);
    nvgFillColor(ctx, nvgRGBA(255,255,255, 200));
    nvgText(ctx, posX + 10, posY + 20, notification.c_str(), NULL);

    return UI_NO_EVENT;
}

std::string UserInterface::getInput() {
    return soundCloudInput.data();
}








