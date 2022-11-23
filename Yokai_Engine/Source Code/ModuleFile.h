#pragma once

#include "Module.h"
#include "Globals.h"

#include "Re_Mesh.h"

#include <string>
#include <vector>

#define ASSETS_PATH "Assets/"

#define LIBRARY_PATH "Library/"
#define MESHES_PATH "Library/Meshes/"
#define TEXTURES_PATH "Library/Textures/"
#define SCENES_PATH "Library/Scenes/"
#define SCRIPTS_PATH "Library/Scripts/"
#define SHADERS_PATH "Library/Shaders/"
#define MATERIALS_PATH "Library/Materials/"

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

	static RE_TYPE FS_GetResourceType(const std::string& filename);

	static std::vector<std::string> FS_GetAllFiles(std::string path);

	bool new_file = false;

	// custom format
	void YK_CreateLibrary();

	char* YK_SaveMesh(uint& size, std::vector<VertexInfo>* vertices, std::vector<uint>* indices);
	void YK_LoadMesh(const char*);

};