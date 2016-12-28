
#ifndef PROJECT_MPEGDECODER_H
#define PROJECT_MPEGDECODER_H
// -------------- MACROS
#define PCMSIZE 4096
#define MP3SIZE PCMSIZE
// -------------- INCLUDES
#include <lame.h>
#include <string>
#include <vector>

class Decoder {

    // - Lame
    mp3data_struct audioData;
    lame_t lame;
    hip_t hip;

    int mpegLength, sampleCount;

    // - Buffers
    unsigned char mpegBuffer[MP3SIZE];

    short pcmL[PCMSIZE];
    short pcmR[PCMSIZE];

public:
    Decoder();
    ~Decoder();
    std::vector<float> decode(std::string& mp3Data);
    unsigned int getPlaybackTime() const;
    unsigned int getTotalSamples() const;
    unsigned int getChannels() const;
    unsigned int getSampleRate() const;
};


#endif //PROJECT_MPEGDECODER_H
