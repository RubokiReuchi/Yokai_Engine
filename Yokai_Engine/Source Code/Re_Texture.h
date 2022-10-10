#pragma once

#include "Globals.h"
#include <string>

class Re_Texture
{
public:
	Re_Texture();
	~Re_Texture();

	std::string name;
	uint OpenGL_id;
};