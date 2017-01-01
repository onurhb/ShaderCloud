#define NANOVG_GL3_IMPLEMENTATION
// -------------- INCLUDES
#include "UserInterface.h"
#include <nanovg_gl.h>
#include <iostream>


UserInterface::UserInterface(GLFWwindow *window)
:io(ImGui::GetIO())
{
    // - Window
    this->window = window;

    // - Interface
    ctx = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);

    if (!ctx) {
        std::cout << "Could not load NanoVG" << std::endl;
        return;
    }

    // - ImGui
    ImGui_ImplGlfwGL3_Init(window, true);
    io.Fonts->AddFontFromFileTTF("Ubuntu-M.ttf", 13.0f);
}

UserInterface::~UserInterface() {
    nvgDeleteGL3(ctx);
    ImGui_ImplGlfwGL3_Shutdown();
}

void UserInterface::render(double timestamp) {
    ImGui_ImplGlfwGL3_NewFrame();
    nvgBeginFrame(ctx, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_WIDTH / WINDOW_HEIGHT);
    {

        renderLeftPanel();
        renderPanelContent();

    }
    nvgEndFrame(ctx);
    ImGui::Render();
}

// -------------------------------------------------------------------------- COMPONENTS

void UserInterface::renderLeftPanel() {

    NVGpaint shadowPaint = nvgBoxGradient(ctx, 256, 0, 0, WINDOW_HEIGHT, 0, 10, nvgRGBA(00, 00, 00, 200),
                                          nvgRGBA(10, 10, 10, 220));

    nvgBeginPath(ctx);
    nvgRect(ctx, 0, 0, 256, WINDOW_HEIGHT);
    nvgFillPaint(ctx, shadowPaint);
    nvgFill(ctx);

}

void UserInterface::renderPanelContent() {
    ImGui::Begin("Test");
    ImGui::Text("Hello World");
    ImGui::End();
}



