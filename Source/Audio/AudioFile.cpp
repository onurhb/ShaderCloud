
// -------------- INCLUDES
#include "AudioFile.h"
#include <iostream>
#include <fstream>


/**
 * Reads from file and fills buffers
 * @param filePath : where the file resides
 * @return true/false
 */
bool AudioFile::loadFile(std::string filePath) {
#if DEBUG_INFO
    printf("File: Loding file.\n");
#endif

    // - Use MPEG decoder if MP3
    std::string extension = filePath.substr(filePath.find_last_of(".") + 1);
    if(!extension.compare("mp3")) {
        // - Read
        std::ifstream file(filePath, std::ios::binary);
        std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
        return decodeMPEG(str);
    }

    // - Use SND library
    file = SndfileHandle(filePath);

    // - Error checking
    if (file.error()) return false;

    // - Information
    audioData.sampleRate = (unsigned int) file.samplerate();
    audioData.channels = (unsigned int) file.channels();
    audioData.totalSamples = (unsigned int) (file.frames() * audioData.channels);
    audioData.playbackTime = (audioData.totalSamples / audioData.sampleRate) / audioData.channels;

    // - Fill memory
    audioData.samples = std::vector<float>(getTotalSamples(), 0.0f);
    file.read(audioData.samples.data(), audioData.totalSamples);

    return true;
}

/**
 * Decodes the MP3 data
 * @param data
 * @return
 */
bool AudioFile::decodeMPEG(std::string& data) {
#if DEBUG_INFO
    printf("File: Decoding file.\n");
#endif
    // - Decode
    audioData.samples = mpegDecoder.decode(data);
    if(audioData.samples.empty()) return false;
    audioData.sampleRate = mpegDecoder.getSampleRate();
    audioData.channels = mpegDecoder.getChannels();
    audioData.totalSamples = mpegDecoder.getTotalSamples();
    audioData.playbackTime = (audioData.totalSamples / audioData.sampleRate) / audioData.channels;
    return true;
}

unsigned int AudioFile::getSampleRate() const {
    return audioData.sampleRate;
}


unsigned int AudioFile::getChannels() const {
    return audioData.channels;
}

unsigned int AudioFile::getPlaybackTime() const {
    return audioData.playbackTime;
}

unsigned int AudioFile::getTotalSamples() const {
    return audioData.totalSamples;
}

std::vector<float> AudioFile::getSamples() {
    return audioData.samples;
}

void AudioFile::free() {
    this->audioData.samples.clear();
}
