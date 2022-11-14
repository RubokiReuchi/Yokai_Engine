#pragma once

#include "Module.h"
#include "Globals.h"

#include <string>
#include <vector>

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

	static bool FS_Exists(const std::string file);

	static bool FS_MakeDir(const std::string dir);

	static bool FS_IsDirectory(const std::string file);

	static std::string FS_NormalizePath(const std::string path);

	static std::string FS_UnNormalizePath(const std::string path);

	static bool FS_AddPathToFileSystem(const std::string path);

	static uint FS_Load(const std::string filePath, char** buffer);

	static uint FS_Save(const std::string filePath, char* buffer, uint size, bool append);

	static uint FS_Copy(const std::string src, const std::string des, bool replace = true);

	static FileTree* FS_GetFileTree(std::string path, FileTree* parent = nullptr);

	static std::string FS_GetFileName(const std::string file, bool getExtension = true);

	static std::string FS_GetFilePath(const std::string file, bool fileIncluded = true);

	static RE_TYPE FS_GetResourceType(const std::string& filename);

	static std::vector<std::string> FS_GetAllFiles(std::string path);

	bool new_file = false;
};