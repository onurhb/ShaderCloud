
// --------------------- INCLUDES
#include "Playback.h"

/**
 * Initialize playback
 */
Playback::Playback() {
}

/**
 * Free memory and cleanup
 */
Playback::~Playback() {
    destroyStream();
    delete[] audioData.samples;
}

/**
 * RtAudio Callback
 * One frame should have left and right channel for each frame if channel amount is two
 * @param outputBuffer  : this buffer is for playing sound, send samples to this buffer
 * @param inputBuffer   : this buffer gives us access to the samples recorded from our input device
 * @param streamTime    : time in ms elapsed
 * @param status        : started/stopped playing
 * @param userData      : struct for accessing data
 */
int callBack(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime,
             RtAudioStreamStatus status, void *userData) {

    auto *out = static_cast<float *>(outputBuffer);
    auto *data = static_cast<Playback::AUDIODATA *>(userData);

    if (data->totalSamples - data->playedSamples < nBufferFrames) {
        data->samplesPtr = &data->samples[0];
        data->playedSamples = 0;
    }

    // - Feed with data
    for (int i = 0; i < nBufferFrames; i++) {
        // - For each channel
        for (int c = 0; c < data->channels; c++) {
            *out++ = *data->samplesPtr++ * data->volume;
        }
    }

    data->playedSamples += nBufferFrames;
    return false;
}

/**
 * Setup RtAudio stream
 */
bool Playback::openStream(unsigned int sampleRate, unsigned int channels) {
    setPlaybackTime((audioData.totalSamples / sampleRate) / channels);
    // - Will destro stream if already open
    destroyStream();
    // - Reset current stream
    resetStream();

    RtAudio::StreamParameters oParameters;
    // - Output parameters
    oParameters.deviceId = rtAudio.getDefaultOutputDevice();
    oParameters.nChannels = audioData.channels = channels;

    // - Sampling rate in hz
    audioData.sampleRate = sampleRate;

    // - Use a frequencyBins of BUFFER_FRAME_SIZE, we need to feed callback with X amount of samples everytime!
    unsigned int nBufferFrames = AUDIO_BUFFER_FRAME_SIZE;

    RtAudio::StreamOptions options;
    options.flags = RTAUDIO_SCHEDULE_REALTIME;
    options.flags = RTAUDIO_MINIMIZE_LATENCY;
    options.flags = RTAUDIO_HOG_DEVICE;

    // - Creating RtAudio stream for playback
    try {
        rtAudio.openStream(&oParameters, NULL, RTAUDIO_FLOAT32, sampleRate, &nBufferFrames, &callBack,
                           static_cast<void *>(&audioData), &options);
        status = PLAYBACK_INITIALIZED;
    }
    catch (RtAudioError &e) {
        std::cerr << e.getMessage() << std::endl;
        return false;
    }

    return true;
}

/**
 * @brief Analyzes currently playing window and returns the spectrum using FFT
 * @return
 */
std::complex<float> *Playback::getSpectrum() {
    if (status != PLAYBACK_PLAYING) return NULL;

    int channels = audioData.channels;
    int totalSamples = AUDIO_BUFFER_FRAME_SIZE;

    int k = 0;
    for (int i = 0; i < totalSamples; i += channels) {
        // - Fill spectrum buffers
        spectrum[k++] = audioData.samplesPtr[i] * 0.5f * (1.0f - cos(float(2.0f * PI * i / FFT_BUFFER_SIZE)));

    }
    return fft.forwardFFT(spectrum);
}

/**
 * Starts the stream. Will play until everything is done.
 * @return true if sucess
 */
bool Playback::playStream() {
    if (rtAudio.isStreamRunning() || !rtAudio.isStreamOpen()) {
        return false;
    };
    try {
        rtAudio.startStream();
    }
    catch (RtAudioError &e) {
        std::cerr << e.getMessage() << std::endl;
        return false;
    }
    this->status = PLAYBACK_PLAYING;
    return true;
}

/**
 * Pause stream.
 * @return true if sucess
 */
bool Playback::pauseStream() {
    if (!rtAudio.isStreamRunning() || !rtAudio.isStreamOpen()) return false;
    try {
        rtAudio.abortStream();
    }
    catch (RtAudioError &e) {
        std::cerr << e.getMessage() << std::endl;
        return false;
    }
    this->status = PLAYBACK_PAUSED;
    return true;
}

/**
 * Kill current stream. All memory will be cleared.
 * This will completely remove the stream. It won't be playable anymore.
 * @return true if destroyed
 */
bool Playback::destroyStream() {
    if (!rtAudio.isStreamOpen()) return false;
    try {
        rtAudio.closeStream();
    }
    catch (RtAudioError &e) {
        std::cerr << e.getMessage() << std::endl;
        return false;
    }
    this->status = PLAYBACK_NOT_INITIALIZE;
    return true;
}

void Playback::resetStream() {
    // - Reset position
    this->audioData.playedSamples = 0;
    audioData.samplesPtr = &audioData.samples[0];
}

/**
 * Seek audio using a simple pointer.
 * If the pointer is out of index, then it won't hurt the stream.
 * @param position
 */
void Playback::seekStream(unsigned int position) {
    if (position < 0) return;
    if (position * AUDIO_BUFFER_FRAME_SIZE > audioData.totalSamples) return;
    audioData.playedSamples = position;
}

/**
 * Seek audio using time in seconds.
 * @param seconds
 */
void Playback::seekStreamSeconds(float seconds) {
    if (seconds < 0) return;
    if (seconds > audioData.playbackTime) return;
    audioData.playedSamples = (unsigned int) seconds * audioData.sampleRate * audioData.channels;
    audioData.samplesPtr = &audioData.samples[audioData.playedSamples];
}

/**
 * Set samples that should be queued.
 * Will also make sure that old samples are deleted.
 * @param samples       : Actual samples
 * @param totalSamples  : Size of samples (not frames).
 */
void Playback::setSamples(float *samples, unsigned int totalSamples) {
    delete[] audioData.samples;
    audioData.totalSamples = totalSamples;
    audioData.samples = new float[totalSamples];
    std::copy(samples, samples + totalSamples, this->audioData.samples);
}

/**
 * @brief This will fill the playback buffer from a vector of samples.
 * @param samples : Actual samples
 */
void Playback::setSamples(std::vector<float> samples) {
    delete[] audioData.samples;
    audioData.totalSamples = samples.size();
    audioData.samples = new float[audioData.totalSamples];
    std::copy(samples.begin(), samples.end(), this->audioData.samples);
}


float Playback::getPlayedTime() const {
    return (this->audioData.playedSamples / this->audioData.channels) / this->audioData.sampleRate;
}


void Playback::setSampleRate(unsigned int sampleRate) {
    this->audioData.sampleRate = sampleRate;
}

void Playback::setTotalSamples(unsigned int totalSamples) {
    this->audioData.totalSamples = totalSamples;
}

void Playback::setPlaybackTime(float playbackTime) {
    this->audioData.playbackTime = playbackTime;
}

void Playback::setChannels(unsigned int channels) {
    this->audioData.channels = channels;
}

void Playback::setVolume(float volume) {
    this->audioData.volume = volume;
}

float Playback::getVolume() const {
    return this->audioData.volume;
}

int Playback::getPosition() const {
    return this->audioData.playedSamples;
}

int Playback::getChannels() const {
    return this->audioData.channels;
}

int Playback::getTotalSamples() const {
    return this->audioData.totalSamples;
}

float Playback::getPlaybackTime() const {
    return this->audioData.playbackTime;
}

int Playback::getSampleRate() const {
    return this->audioData.sampleRate;
}

PLAYBACK_STATUS Playback::getStatus() const {
    return this->status;
}

/**
 * Returns the installed version of RtAudio
 * @return : const char* version
 */
std::string Playback::getVersion() const {
    return rtAudio.getVersion();
}



