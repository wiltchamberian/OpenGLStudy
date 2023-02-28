#include "tools.h"
#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <chrono>

std::string readFromFile(const char* pathToFile)
{
    std::string content;
    std::ifstream fileStream(pathToFile, std::ios::in);

    if (!fileStream.is_open()) {
        std::cerr << "Could not read file " << pathToFile << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while (!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

GLuint createShader(const char* src, GLenum type) {
    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);
    int  success = 0;
    char infoLog[512];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        throw std::runtime_error(infoLog);
    }
    return id;
}

GLuint createProgram(GLuint vertex, GLuint fragment) {
    const GLubyte* pVersion = glGetString(GL_VERSION);
    const GLubyte* pGlslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    GLuint id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);

    glLinkProgram(id);
    int success = 0;
    char infoLog[512];
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        throw std::runtime_error(infoLog);
    }

    return id;
}

GLuint createProgram(const char* vertex_path, const char* fragment_path) {
    std::string vertex_content = readFromFile(vertex_path);
    std::string fragment_content = readFromFile(fragment_path);
    auto id1 = createShader(vertex_content.c_str(), GL_VERTEX_SHADER);
    auto id2 = createShader(fragment_content.c_str(), GL_FRAGMENT_SHADER);
    GLuint pro = createProgram(id1, id2);
    glDeleteShader(id1);
    glDeleteShader(id2);
    return pro;
}

double globalMilliseconds()
{
    int			sys_curtime;
    static bool	s_initialized = false;
    static std::chrono::high_resolution_clock::time_point s_timeBase;

    if (!s_initialized) {
        s_timeBase = std::chrono::high_resolution_clock().now();
        s_initialized = true;
    }
    auto curTime = std::chrono::high_resolution_clock().now();
    sys_curtime = std::chrono::duration<double, std::milli>(curTime - s_timeBase).count();

    return sys_curtime;
}