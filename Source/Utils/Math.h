#ifndef PROJECT_MATH_H
#define PROJECT_MATH_H

// -------------- INCLUDES
#include <cmath>
// -------------- MACROS

/*
 * Converts from degree to radians
 */
inline float toRadians(float degree) {
    return 3.1415926f * degree / 180.0f;
}

/*
 * Convert to dB
 */
inline float todB(float value, float sensevity, float scale) {
    value = 10.0f * std::log10(value / sensevity) * scale;
    if (value < 0.0f) value = 0.0f;
    return value;
}

/*
 * Interpolates between two values
 */
inline float interpolate(float last, float current, float smoothFactor) {
    return smoothFactor * last + (1.0f - smoothFactor) * current;
}

#endif //PROJECT_MATH_H
