#pragma once

#include "Globals.h"
#include "OpenGL.h"

#include <string>

class TextureImporter
{
public:

	static uint ImportTexture(std::string path);
	static uint CheckerImage();
	static uint ImportTextureSTBI(std::string path);
};