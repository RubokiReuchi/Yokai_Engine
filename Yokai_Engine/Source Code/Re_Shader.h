#pragma once

#include "OpenGL.h"

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

class Re_Shader
{
public:
    unsigned int program_id;

    Re_Shader();
    Re_Shader(const char* vertexPath, const char* fragmentPath);

    void CreateShader(const char* vertexPath, const char* fragmentPath);
};