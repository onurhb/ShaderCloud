// -------------- INCLUDES
#include <imgui.h>
#include "ShaderVisualizer.h"

ShaderVisualizer::ShaderVisualizer()
        : texture(WIDTH, HEIGHT) {

    this->data = new GLubyte[WIDTH * HEIGHT];
    for (int i = 0; i < WIDTH; ++i) this->data[i] = (GLubyte) 0.0f;
    texture.bindTexture();
    texture.loadData(data);
    texture.unbindTexture();

    // - Temporary
    shader = new Shader();

    initialize();
}

ShaderVisualizer::~ShaderVisualizer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    delete shader;
    delete[] data;
}

/**
 * Initialize buffers. 
 */
void ShaderVisualizer::initialize() {
    // - Initialize shader
    shader->loadFromFile("", "shader.frag");
    shader->bindShader();
    shader->setUniform2f("iResolution", glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT));

    // - Initialize a big rectangle to render on
    GLfloat vertices[] = {
            // First triangle
            1.0f, 1.0f, 0.0f,  // Top Right
            1.0f, -1.0f, 0.0f,  // Bottom Right
            -1.0f, 1.0f, 0.0f,  // Top Left
            // Second triangle
            1.0f, -1.0f, 0.0f,  // Bottom Right
            -1.0f, -1.0f, 0.0f,  // Bottom Left
            -1.0f, 1.0f, 0.0f   // Top Left
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

/**
 * Renders a big rectangle and use the shader.
 * @param time : current time. Necessary for passing iGlobalTime to shader.
 */
void ShaderVisualizer::render(double time) {
    shader->bindShader();
    shader->setUniform1f("iGlobalTime", float(time));
    texture.bindTexture();
    texture.loadSubData(data, WIDTH, HEIGHT);
    {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    texture.unbindTexture();
    shader->unbindShader();
}

/**
 * Sets shader. If one shader is not provided, then a default shader will be used.
 * @param fpath : path to fragment
 * @param vpath : path to vertex
 */
void ShaderVisualizer::setShader(const char *vpath, const char *fpath) {
    // - Handle shader if already exists
    if (this->shader == nullptr) this->shader = new Shader();
    else delete this->shader;
    // - If no vertex shader is provided, only set the fragment shader
    this->shader->loadFromFile(vpath, fpath);
}


void ShaderVisualizer::setSpectrum(std::complex<float> *spectrum) {
    if (!spectrum) return;
    for (int i = 0; i < WIDTH; ++i) {
        float val = std::abs(spectrum[i]) / FFT_BUFFER_SIZE * 2.0f * 255.0f;
        val = todB(val, sensevity, scale);
        data[i] = (GLubyte) interpolate(lastValues[i], val, smooth);
        lastValues[i] = val;
    }
}

void ShaderVisualizer::renderWidget() {
    if (ImGui::Begin("Shadervisualizer")) {
        ImGui::SliderFloat("Sensevity", &sensevity, 0.01, 1.0);
        ImGui::SliderFloat("Scale", &scale, 0.01, 100.0);
        ImGui::SliderFloat("Smooth", &smooth, 0.1, 1.0);
    }
    ImGui::End();
}