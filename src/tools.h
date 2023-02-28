#pragma once
#include "glad/glad.h"
#include <string>

std::string readFromFile(const char* pathToFile);
GLuint createShader(const char* src, GLenum type);
GLuint createProgram(GLuint vertex, GLuint fragment);
GLuint createProgram(const char* vertex_path, const char* fragment_path);

double globalMilliseconds();