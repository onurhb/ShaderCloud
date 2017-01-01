
#ifndef PROJECT_TEXTURE_H
#define PROJECT_TEXTURE_H

// -------------- INCLUDES
#include <glad/glad.h>
#include <iostream>

/**
 * Texture wrapper for OpenGL textures
 */
class Texture {
    GLuint textureID;
    int width, height;
    GLenum format, wrap, filter;

public:
    inline Texture(int width, int height, GLenum wrap = GL_CLAMP_TO_EDGE, GLenum filter = GL_LINEAR);
    inline ~Texture();

    inline void bindTexture() const;
    inline void unbindTexture() const;
    inline void loadData(GLubyte *data, GLenum format = GL_LUMINANCE);
    inline void loadSubData(GLubyte *data, int width, int height, int offsetX = 0, int offsetY = 0);
};

Texture::Texture(int width, int height, GLenum wrap, GLenum filter)
        : width(width), height(height), wrap(wrap), filter(filter) {

    // - Generate the texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);

}

Texture::~Texture() {
    if(textureID) glDeleteTextures(1, &textureID);
}

/**
 * Loads data to texture.
 * @param data
 * @param format
 */
void Texture::loadData(GLubyte *data, GLenum format) {
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    this->format = format;
}


/**
 * Loads a chunk of data to already created texture.
 * @param data : pixel values
 * @param width : width of texture
 * @param height : height of texture
 * @param offsetX : offset X
 * @param offsetY : offset Y
 */
void Texture::loadSubData(GLubyte *data, int width, int height, int offsetX, int offsetY) {
    glTexSubImage2D(GL_TEXTURE_2D, 0, offsetX, offsetY, width, height, format, GL_UNSIGNED_BYTE, data);
}

/**
 * Binds texture for usage.
 */
void Texture::bindTexture() const {
    glBindTexture(GL_TEXTURE_2D, textureID);
}

/**
 * Unbind after usage.
 */
void Texture::unbindTexture() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

#endif //PROJECT_TEXTURE_H
