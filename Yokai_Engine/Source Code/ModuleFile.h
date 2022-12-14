#pragma once

#include "Module.h"
#include "Globals.h"
#include "ResourceManager.h"
#include "MathGeoLib/include/MathGeoLib.h"

#include <string>
#include <vector>

#define ASSETS_PATH "Assets/"
#define SCRIPTS_PATH "Assets/Scripts/"

#define LIBRARY_PATH "Library/"
#define MODELS_PATH "Library/Models/"
#define MESHES_PATH "Library/Meshes/"
#define TEXTURES_PATH "Library/Textures/"
#define SCENES_PATH "Library/Scenes/"
#define MATERIALS_PATH "Library/Materials/"

class FileTree;
struct VertexInfo;

struct ModelYK
{
	std::vector<std::string> children_paths;
	std::vector<float3> positions;
	std::vector<float3> scales;
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

	static std::string FS_GetExtension(const std::string& file);

	static std::vector<std::string> FS_GetAllFiles(std::string path);

	static std::vector<std::string> FS_RemoveExtensions(std::vector<std::string> paths);
	static std::string FS_RemoveExtension(std::string path);

	static std::string FS_GetLastModifyDate(std::string path);

	bool new_file = false;

	// custom format
	void YK_CreateLibrary();

	void YK_SaveModel(std::string path, std::vector<std::string> children_paths, std::vector<float3> positions, std::vector<float3> scales);
	ModelYK YK_LoadModel(std::string path);
	char* YK_SaveMesh(uint& size, std::vector<VertexInfo> vertices, std::vector<uint> indices);

	void YK_SaveMetaData(std::string file_name, std::string file_reference);
	static std::string YK_GetMetaInfo(std::string path, int line);
};