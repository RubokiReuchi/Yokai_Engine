#pragma once

#include "OpenGL.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Re_Shader
{
public:
    unsigned int program_id;

    Re_Shader();
    Re_Shader(const char* vertexPath, const char* fragmentPath);

    // Create Shader
    void CreateShader(const char* vertexPath, const char* fragmentPath);

    void Bind();
    
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetFloat3(const std::string& name, float v1, float v2, float v3) const;
    void SetFloat4(const std::string& name, float v1, float v2, float v3, float v4) const;
    void SetMatFloat4v(const std::string& name, const float* value);

};