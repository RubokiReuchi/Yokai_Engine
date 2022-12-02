#pragma once

#include "Globals.h"
#include "OpenGL.h"
#include "Re_Texture.h"

#include <map>
#include <vector>

class M_Texture
{
public:

	struct BindedTextureInfo
	{
		BindedTextureInfo(uint tID, uint oglID) : Texture_id(tID), OpenGL_slot_id(oglID) {}
		uint Texture_id;
		uint OpenGL_slot_id;
	};

	static float BindTexture(uint texture);
	static void UnBindTextures();

	static void DeleteTextures();

	static uint bindedTextures;
	static std::map<uint, Re_Texture> loaded_textures;
	static std::map<std::string, uint> usedPaths;
	static std::map<std::string, uint> texture_image;
	static std::vector<BindedTextureInfo> bindedTexturesInfo;
};