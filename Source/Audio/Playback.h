
#ifndef PROJECT_PLAYBACK_H
#define PROJECT_PLAYBACK_H

// ---------------- INCLUDES
#include <RtAudio.h>
#include "../Algorithm/FFT.h"
#include "../config.h"


enum PLAYBACK_STATUS {PLAYBACK_PLAYING, PLAYBACK_PAUSED, PLAYBACK_NOT_INITIALIZE, PLAYBACK_INITIALIZED};

class Playback {

    // - RtAudio for cross-platform playback
    RtAudio rtAudio;
    // - FFT
    FFT fft;
    // - Spectrum
    std::complex<float> spectrum[FFT_BUFFER_SIZE];

    // - Data related to audio
    struct AUDIODATA {
        unsigned int sampleRate         = 1;
        unsigned int channels           = 1;
        float playbackTime              = 0.0f;
        unsigned int totalSamples       = 0;
        float *samples                  = nullptr;           // - Samples
        float *samplesPtr               = nullptr;
        unsigned int playedSamples    = 0;                 // - Pointer to position in frequencyBins
        float volume                    = 1.0f;
    } audioData;

    // - Status
    PLAYBACK_STATUS status = PLAYBACK_NOT_INITIALIZE;

private:
    // - Called everytime it needs chunk of samples
    friend int callBack(void *outputBuffer, void *inputBuffer,
                        unsigned int nBufferFrames, double streamTime,
                        RtAudioStreamStatus status, void *userData);


public:
    Playback();
    ~Playback();

    // - Stream
    bool openStream(unsigned int sampleRate, unsigned int channels);
    bool playStream();
    bool pauseStream();
    void resetStream();
    void seekStream(unsigned int position);
    void seekStreamSeconds(float seconds);
    bool destroyStream();

    // ------------------------------- Getters
    std::string getVersion() const;
    float getVolume() const;
    int getPosition() const;
    int getChannels() const;
    int getTotalSamples() const;
    int getSampleRate() const;
    float getPlaybackTime() const;
    float getPlayedTime() const;
    PLAYBACK_STATUS getStatus() const;
    std::complex<float>* getSpectrum();
    // ------------------------------- Setters
    void setSampleRate(unsigned int sampleRate);
    void setTotalSamples(unsigned int totalSamples);
    void setPlaybackTime(float playbackTime);
    void setChannels(unsigned int channels);
    void setSamples(float *samples, unsigned int totalSamples);
    void setSamples(std::vector<float> samples);
    void setVolume(float volume);



};


#endif //PROJECT_PLAYBACK_H
