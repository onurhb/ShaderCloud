// - Default shaders
const char *defaultFragShaderSrc =
        "#version 330 core\n"
                "out vec4 fragColor;\n"
                "void main(){\n"
                "    \tfragColor = vec4(0.1, 0.1, 0.1, 1.0);\n"
                " }";

const char *defaultVertShaderSrc =
        "#version 330 core\n"
                "layout (location = 0) in vec4 position;\n"
                "void main()\n"
                "{\n"
                "     gl_Position = position;\n"
                "}";

// ---------------- INCLUDES
#include <vector>
#include <iostream>
#include <fstream>
#include "Shader.h"
#include <gtc/type_ptr.hpp>
// ----------------

Shader::Shader() {}

Shader::~Shader() {
    glDeleteProgram(shaderID);
}

/**
 * Activates current shader
 */
void Shader::bindShader() const {
    glUseProgram(shaderID);
}

/**
 * Deactivates current shader
 */
void Shader::unbindShader() {
    glUseProgram(0);
}

/**
 * Returns the uniform location
 * @param name : name of the uniform
 */
GLint Shader::getUniformLocation(const GLchar *name) {
    // - First check if name is already cached
    if (cache.count(name) > 0) {
        return cache.find(name)->second;
    }

    // - If not add to cache
    GLint location = glGetUniformLocation(shaderID, name);
    cache[name] = location;
    return location;
}

void Shader::setUniform1f(const GLchar *name, float value) {
    glUniform1f(getUniformLocation(name), value);
}

void Shader::setUniform2f(const GLchar *name, const glm::vec2 &vector) {
    glUniform2f(getUniformLocation(name), vector.x, vector.y);
}

void Shader::setUniform3f(const GLchar *name, const glm::vec3 &vector) {
    glUniform3f(getUniformLocation(name), vector.x, vector.y, vector.z);
}

void Shader::setUniform4f(const GLchar *name, const glm::vec4 &vector) {
    glUniform4f(getUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
}

void Shader::setUniformMat4f(const GLchar *name, const glm::mat4 &mat) {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

/**
 * Returns currently active uniforms used by this shader
 */
void Shader::getActiveUniforms() const {

    GLint count;
    GLint size;
    GLenum type;

    const GLsizei bufSize = 16;
    GLchar name[bufSize];
    GLsizei length;

    glGetProgramiv(shaderID, GL_ACTIVE_UNIFORMS, &count);

    for (int i = 0; i < count; i++) {
        glGetActiveUniform(shaderID, static_cast<GLuint>(i), bufSize, &length, &size, &type, name);

        std::cout << "Uniform :" << i << " Type: " << type << " Name: " << name << std::endl;
    }

}

/*
 * Reads shader file and returns raw text
 */
std::string Shader::readFile(std::string filePath) const {

    if(filePath.empty()) return "";

    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    // - Reads for each line and appends to content
    while (!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();

    return content;
}

/**
 * Reads shader program from file and creates a shader program.
 * When a path is not given (is "") then a default shader will be used.
 * @param vpath : path to vertex shader
 * @param fpath  : path to fragment shader
 * @return success/unsuccessful
 */
bool Shader::loadFromFile(std::string vpath, std::string fpath) {
    // - Read shader files
    std::string vertShaderSrc = readFile(vpath);
    std::string fragShaderSrc = readFile(fpath);

    return loadFromString(vertShaderSrc, fragShaderSrc);
}

/**
 * Loads directly from string. If one is not provided, then use the default one.
 * @param vertShaderSrc : vertex shader string
 * @param fragShaderSrc : fragment shader string
 * @return success/unsuccessful
 */
bool Shader::loadFromString(std::string vertShaderSrc, std::string fragShaderSrc) {
    
    const char *vertShaderStr = vertShaderSrc.empty() ? NULL :vertShaderSrc.c_str();
    const char *fragShaderStr = fragShaderSrc.empty() ? NULL :fragShaderSrc.c_str();
    
    // - If both are provided
    if(vertShaderStr && fragShaderStr)          return load(vertShaderStr, fragShaderStr);
    // - If vertex shader is not provided
    else if(!vertShaderStr && fragShaderStr)    return load(defaultVertShaderSrc, fragShaderStr);
    // - If fragment shader is not provided
    else if(!fragShaderStr && vertShaderStr)    return load(vertShaderStr, defaultFragShaderSrc);
    // - If vertex and fragment shader are not provided
    else                                        return load(defaultVertShaderSrc, defaultFragShaderSrc);
}

/**
 * Loads shader sources.
 * @param vertShaderSrc : vertex shader string
 * @param fragShaderSrc : fragment shader string
 * @return success/unsuccessful
 */
bool Shader::load(const char *vertShaderSrc, const char *fragShaderSrc) {
    GLint result;

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    // - Compile vertex shader
    glShaderSource(vertShader, 1, &vertShaderSrc, nullptr);
    glCompileShader(vertShader);

    // - Check vertex shader
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        GLint length;
        glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> error(length);
        glGetShaderInfoLog(vertShader, length, &length, &error[0]);
        std::cout << "Failed to compile vertex shader!" << std::endl << &error[0] << std::endl;
        glDeleteShader(vertShader);
        return false;
    }

    // - Compile fragment shader
    glShaderSource(fragShader, 1, &fragShaderSrc, nullptr);
    glCompileShader(fragShader);

    // - Check fragment shader
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        GLint length;
        glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> error(length);
        glGetShaderInfoLog(fragShader, length, &length, &error[0]);
        std::cout << "Failed to compile fragment shader!" << std::endl << &error[0] << std::endl;
        glDeleteShader(fragShader);
        return false;
    }

    // - Create the shader program
    shaderID = glCreateProgram();

    // - Attach shaders to program
    glAttachShader(shaderID, vertShader);
    glAttachShader(shaderID, fragShader);
    glLinkProgram(shaderID);

    // - Check if program is succesfully compiled
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    if (result == GL_FALSE) {
        GLint length;
        glGetProgramiv(shaderID, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> error(length);
        glGetProgramInfoLog(shaderID, length, &length, &error[0]);
        std::cout << "Failed to compile shader!" << std::endl << &error[0] << std::endl;
        glDeleteProgram(shaderID);
        return false;
    }

    // - Free
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    return true;
}


