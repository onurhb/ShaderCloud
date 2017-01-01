
// ---------------- INCLUDES
#include "Application.h"


Application::Application():
window(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT),
userInterface(window.getWindow()),
updateThread(false)
{

}

Application::~Application() {}

/**
 * @brief Requests external file using HTTP
 * @param URL : link to file
 */
void Application::getExternalFile(std::string URL) {
    // - Download the actual mp3
    std::string stream = http.getSoundCloudStream(URL);

    if (!stream.empty()) {
        if (audioFile.decodeMPEG(stream)) {
            // - File playback
            playback.pauseStream(); // - Pause stream temporary
            playback.setSamples(audioFile.getSamples());
            playback.openStream(audioFile.getSampleRate(), audioFile.getChannels());
            playback.playStream();

        }

    }

    audioThread.setDone();
}

/**
 * Opends an audio file. Threaded.
 * @param path : path to file
 */
void Application::getLocalFile() {

    std::string path = fileDialog.askPath("flac,wav,ogg,mp3");
    if (path.empty()) {
        return audioThread.setDone();
    }

    // - Notify

    // - Open file
    if (audioFile.loadFile(path)) {

        // - File playback
        playback.pauseStream();
        playback.setSamples(audioFile.getSamples());
        playback.openStream(audioFile.getSampleRate(), audioFile.getChannels());
        playback.playStream();


    }

    audioThread.setDone();
}


/**
 * Handling rendering here...
 */
void Application::render() {
    window.clear();
    {
        shaderVisualizer.render(glfwGetTime());
        userInterface.render(glfwGetTime());
    }
    window.update();
}

/**
 * Handling update here...
 */
void Application::update(bool &closed) {
    double time = glfwGetTime();
    double ns = 1.0 / 60.0;
    while (!closed) {
        if (glfwGetTime() - time > ns) {
            time += ns;
            shaderVisualizer.setSpectrum(playback.getSpectrum());
        }
    }
}


/**
 * When called, the application will run until window has been closed or destroyed
 * @return Success / Unsuccess
 */
void Application::loop() {
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





