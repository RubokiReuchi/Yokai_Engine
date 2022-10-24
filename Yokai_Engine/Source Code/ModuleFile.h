#pragma once

#include "Module.h"
#include "Globals.h"

#include <string>

#define ASSETS_PATH "Assets/"

class FileTree;

enum class RE_TYPE
{
	UNDEFINED,
	MESH,
	TEXTURE,
};

class ModuleFile : public Module
{
public:
	ModuleFile(bool start_enabled = true);

	~ModuleFile();

	// Static functions
	static bool S_Exists(const std::string file);

	static bool S_MakeDir(const std::string dir);

	static bool S_IsDirectory(const std::string file);

	static std::string S_NormalizePath(const std::string path);

	static std::string S_UnNormalizePath(const std::string path);

	// can be path or zip
	static bool S_AddPathToFileSystem(const std::string path);

	static uint S_Load(const std::string filePath, char** buffer);

	static uint S_Save(const std::string filePath, char* buffer, uint size, bool append);

	static uint S_Copy(const std::string src, const std::string des, bool replace = true);


	static FileTree* S_GetFileTree(std::string path, FileTree* parent = nullptr);

	static std::string S_GetFileName(const std::string file, bool getExtension = true);

	static RE_TYPE S_GetResourceType(const std::string& filename);
};