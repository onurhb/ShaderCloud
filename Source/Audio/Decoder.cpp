// -------------- INCLUDES
#include "Decoder.h"
#include <sstream>
#include <iostream>


Decoder::Decoder()
        :lame(lame_init())
{
    if (lame_init_params(lame) == -1) std::cerr << "Error, could not initialize LAME." << std::endl;
    lame_set_decode_only(lame, 1);
}

Decoder::~Decoder() {
    lame_close(lame);
}


/**
 * Remember to delete samples. If you keep decode it will leak memory.
 * freeMemory() should be called before decoding again. Remember to copy the samples as they will be also deleted.
 * @param mp3Data
 * @return
 */
std::vector<float> Decoder::decode(std::string &mp3Data) {
    // - Temporary vector
    std::vector<float> temp;

    // - Initialize hip
    hip = hip_decode_init();

    // - Create a string stream
    std::istringstream inputFile(mp3Data);

    // - Decode
    do {
        inputFile.read((char *) mpegBuffer, MP3SIZE);
        mpegLength = inputFile.gcount();

        sampleCount = hip_decode1_headers(hip, mpegBuffer, (size_t) mpegLength, pcmL, pcmR, &audioData);

        if (sampleCount > 0) {
            for (int i = 0; i < sampleCount; i++) {
                temp.push_back((float) pcmL[i] / (float) 32768);
                if (audioData.stereo == 2) temp.push_back((float) pcmR[i] / (float) 32768);
            }
        }

    } while (sampleCount > 0 || !inputFile.eof());

    hip_decode_exit(hip);

    return temp;

}

unsigned int Decoder::getPlaybackTime() const {
    return audioData.nsamp / audioData.samplerate;
}

unsigned int Decoder::getTotalSamples() const {
    return audioData.nsamp * audioData.stereo;
}

unsigned int Decoder::getChannels() const {
    return (unsigned int) audioData.stereo;
}

unsigned int Decoder::getSampleRate() const {
    return (unsigned int) audioData.samplerate;
}

