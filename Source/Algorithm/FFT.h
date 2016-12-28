
#ifndef PROJECT_FFT_H
#define PROJECT_FFT_H
// -------------- INCLUDES
#include <kiss_fft.h>
#include <complex.h>
#include "../config.h"


/**
 * This is a wrapper for KissFFT
 */

class FFT {


    // - FFT
    kiss_fft_cfg fwd;

public:

    /**
     * Allocated memory for KissFFT
     * @param N : total samples
     */
    FFT() : fwd(kiss_fft_alloc(FFT_BUFFER_SIZE, 0, NULL, NULL)) {}

    /**
     * Clears KissFFT memory
     */
    ~FFT() { kiss_fft_free(fwd); }

    /**
     * @brief Execute forward FFT on complex samples
     * @param samples
     * @return
     */
    std::complex<float> *forwardFFT(std::complex<float> *samples) {
        kiss_fft(fwd, (kiss_fft_cpx *) samples, (kiss_fft_cpx *) samples);
        return samples;
    }

};


#endif //PROJECT_FFT_H
