#ifndef PROJECT_APPLICATION_H
#define PROJECT_APPLICATION_H

// --------------- MACROS
#define DEBUG 1
// --------------- INCLUDES
#include "Graphics/Window.h"
#include "Graphics/Shader.h"
#include "Graphics/UserInterface.h"
#include "Audio/Playback.h"
#include "Audio/AudioFile.h"
#include "Utils/Thread.h"
#include "Graphics/ShaderVisualizer.h"
#include "config.h"
#include "Utils/FileDialog.h"
#include "Utils/HTTP.h"
// ---------------

class Application {

    Window window;

    // - Components
    ShaderVisualizer shaderVisualizer;
    UserInterface userInterface;
    FileDialog fileDialog;
    AudioFile audioFile;
    Playback playback;
    HTTP http;

    // - Threads
    Thread audioThread;
    Thread updateThread;

    // - Notification
    std::string notification;


public:
    Application();
    ~Application();
    void loop();

private:
    void render();
    void update(bool &closed);
    void getLocalFile();
    void getExternalFile(std::string URL);
};


#endif //PROJECT_APPLICATION_H
