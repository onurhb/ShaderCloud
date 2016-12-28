
// ---------------- INCLUDES
#include "Application.h"


Application::Application()
        : window(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, "icon.png"), updateThread(NULL, false) {

    userInterface.initialize(window.getWindow());
}

Application::~Application() {

}

/**
 * @brief Requests external file using HTTP
 * @param URL : link to file
 */
void Application::getExternalFile(std::string URL) {
    userInterface.setNotification("Getting track. Please wait.\n", -1.0f);
    // - Download the actual mp3
    std::string stream = http.getSoundCloudStream(URL);
    if (!stream.empty()) {
        userInterface.setNotification("Decoding the track.\n", -1.0f);
        if (audioFile.decodeMPEG(stream)) {
            // - File playback
            playback.pauseStream(); // - Pause stream temporary
            playback.setSamples(audioFile.getSamples());
            playback.openStream(audioFile.getSampleRate(), audioFile.getChannels());
            playback.playStream();
            userInterface.setNotification("Playing!\n", 6.0f);

        }else userInterface.setNotification("Failed to decode track!\n", 2.0f);

    } else {
        userInterface.setNotification("Failed. Try another track!\nThis might be a private track.", 6.0f);
    }

    audioThread.setDone();
}

/**
 * Opends an audio file. Threaded.
 * @param path : path to file
 */
void Application::getLocalFile() {
    std::string path;

    // - Ask for file
    if (fileDialog.askPath("flac,wav,ogg,mp3") == DIALOG_OK) {
        path = fileDialog.getPath();

        // - Notify
        userInterface.setNotification("Decoding : \n" + path.substr(path.find_last_of("/\\") + 1));

        // - Open file
        if (audioFile.loadFile(path)) {

            // - File playback
            playback.pauseStream();
            playback.setSamples(audioFile.getSamples());
            playback.openStream(audioFile.getSampleRate(), audioFile.getChannels());
            playback.playStream();

            userInterface.setNotification("Playing: \n" + path.substr(path.find_last_of("/\\") + 1), 6.0f);

        } else {
            userInterface.setNotification("Error, could not open: \n" + path.substr(path.find_last_of("/\\") + 1),
                                          3.0f);
        }
    }
    audioThread.setDone();
}


/**
 * Handling rendering here...
 */
void Application::render() {
    window.clear();
    userInterface.preRender();
    {

        shaderVisualizer.setSpectrum(playback.getSpectrum());
        shaderVisualizer.render(glfwGetTime());
        shaderVisualizer.renderWidget();

        userInterface.renderNotification();
        userInterface.renderPlayback(playback, openFileDialog);

        // - If filedialog is open
        if (openFileDialog) {
            INPUT_OPTIONS option = userInterface.renderFileModal(openFileDialog);

            if (option == LOCAL_INPUT) {
                if (!audioThread.startThread(&Application::getLocalFile, this)) {
                    userInterface.setNotification("Hold on you're too fast...", 2.0);
                }
            } else if (option == SOUNDCLOUD_INPUT) {
                if (!audioThread.startThread(&Application::getExternalFile, this, userInterface.getInput())) {
                    userInterface.setNotification("Hold on you're too fast...", 2.0);
                }
            }
        }
    }
    userInterface.preUpdate();
    window.update();
}

/**
 * Handling update here...
 */
void Application::update(bool &closed) {
    while (!closed){
        shaderVisualizer.setSpectrum(playback.getSpectrum());
    }
}


/**
 * When called, the application will run until window has been closed or destroyed
 * @return Success / Unsuccess
 */
bool Application::loop() {
    // - Thread handling updates
    bool closed = false;
    updateThread.startThread(&Application::update, this, std::ref(closed));
    // - While window is alive
    while (!window.closed()) {
        render();
    }
    closed = true;
    updateThread.join();
}





