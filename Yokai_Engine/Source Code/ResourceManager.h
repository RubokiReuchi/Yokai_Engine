#pragma once

#include "Globals.h"
#include "Console.h"

#include <string>

enum class RE_TYPE
{
	UNDEFINED,
	MESH,
	TEXTURE,
};

class ResourceManager
{
public:
	
	static std::string LoadResource(std::string path);

	static void CheckRemovedFiles();
	static void CheckModifiedFiles();
	
};