
#ifndef PROJECT_SHADERVISUALIZER_H
#define PROJECT_SHADERVISUALIZER_H

// --------------- MACROS
#define WIDTH   FFT_BUFFER_SIZE / 2
#define HEIGHT  1
// -------------- INCLUDES
#include <glad/glad.h>
#include <complex>
#include "Shader.h"
#include "Texture.h"
#include "../config.h"
#include "../Utils/Math.h"


class ShaderVisualizer {

    GLubyte *data;
    Shader *shader;
    float lastValues[WIDTH];
    Texture texture;
    GLuint VAO, VBO;
    float sensevity = 0.1, smooth = 0.80, scale = 1.0;

public:
    ShaderVisualizer();
    ~ShaderVisualizer();
    void initialize();
    void render(double time = 0.0);
    void setShader(const char *vpath = NULL, const char *fpath = NULL);
    void setSpectrum(std::complex<float> *spectrum);
    void renderWidget();
};


#endif //PROJECT_SHADERVISUALIZER_H
