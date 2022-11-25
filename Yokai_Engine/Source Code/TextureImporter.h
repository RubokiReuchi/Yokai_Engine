#pragma once

#include "Globals.h"
#include "OpenGL.h"

#include <string>

class TextureImporter
{
public:
	static uint LoadTexture(std::string path);
	static uint CreateTextureChecker();

private:
	static char* YK_SaveTexture(uint& size, std::string path);
	static uint ImportTexture(std::string path);

};