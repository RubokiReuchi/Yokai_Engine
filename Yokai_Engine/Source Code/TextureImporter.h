#pragma once

#include "Globals.h"
#include "OpenGL.h"

#include <string>

class TextureImporter
{
public:
	static uint CreateTextureChecker();
	static uint ImportTexture(std::string path);
};