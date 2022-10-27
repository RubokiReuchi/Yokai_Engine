#pragma once

#include "Globals.h"
#include "OpenGL.h"

#include <string>

class TextureImporter
{
public:
	static void ImportImage(const char* fileName, char* buffer, int size);

	// Loads a texture from DDS file to OpenGL and returns its id
	static uint Load(char* buffer, int size, int* w, int* h);

	static uint ImportTexture(std::string path);
	static uint CheckerImage();
	static uint ImportTextureSTBI(std::string path);
};