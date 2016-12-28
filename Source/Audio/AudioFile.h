
#ifndef PROJECT_AUDIOFILE_H
#define PROJECT_AUDIOFILE_H


// -------------- INCLUDES
#include <sndfile.hh>
#include "Decoder.h"
#include "../config.h"

class AudioFile {

    // - SndFile
    SndfileHandle file;

    // - MPEG decoder
    Decoder mpegDecoder;

    // - Data
    struct {
        std::vector<float> samples;
        unsigned int sampleRate = 0;
        unsigned int channels = 0;
        unsigned int playbackTime = 0;
        unsigned int totalSamples = 0;

    } audioData;

public:

    bool loadFile(std::string filePath);
    bool decodeMPEG(std::string &data);
    void free();
    // - Getters
    unsigned int getSampleRate() const;
    unsigned int getChannels() const;
    unsigned int getPlaybackTime() const;
    unsigned int getTotalSamples() const;
    std::vector<float> getSamples();

};


#endif //PROJECT_AUDIOFILE_H
