#pragma once

#include "Globals.h"
#include "Re_Texture.h"

#include <map>

class M_Texture
{
public:

	static float BindTexture(uint texture);
	static void UnBindTextures();

	static uint bindedTextures;
	static std::map<uint, Re_Texture> loaded_textures;
	static std::map<std::string, uint> usedPaths;
};