#ifndef PROJECT_SHADERMANAGER_H
#define PROJECT_SHADERMANAGER_H

// ------------------ INCLUDES
#include <glad/glad.h>
#include "glm.hpp"
#include <map>
// ------------------


class Shader {

private:
    // - This shader's ID, if -1 -> not created
    GLuint shaderID;
    // - Dictonary for uniformLocations
    std::map<std::string, GLint> cache;

private:
    GLint getUniformLocation(const GLchar *name);
    bool load(const char *vertShaderSrc, const char *fragShaderSrc);

public:

    Shader();
    ~Shader();
    bool loadFromString(std::string vertShaderSrc,std::string fragShaderSrc);
    bool loadFromFile(std::string vpath, std::string fpath);
    std::string readFile(std::string filePath) const;
    static void unbindShader();
    void bindShader() const;

    // - Uniforms
    void setUniform1f(const GLchar *name, float value);
    void setUniform3f(const GLchar *name, const glm::vec3 &vector);
    void setUniform2f(const GLchar *name, const glm::vec2 &vector);
    void setUniform4f(const GLchar *name, const glm::vec4 &vector);
    void setUniformMat4f(const GLchar *name, const glm::mat4 &mat);
    void getActiveUniforms() const;

};


#endif //PROJECT_SHADERMANAGER_H