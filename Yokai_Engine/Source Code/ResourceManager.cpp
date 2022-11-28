#include "ResourceManager.h"
#include "ModuleFile.h"
#include "MeshImporter.h"
#include "TextureImporter.h"
#include "FileTree.hpp"

std::string ResourceManager::LoadResource(std::string path)
{
	std::string message = "";

	switch (ModuleFile::FS_GetResourceType(path))
	{
	case RE_TYPE::MESH:
		MeshImporter::LoadMesh(path);
		message = "Mesh placed";
		break;
	case RE_TYPE::TEXTURE:
		TextureImporter::LoadTexture(path);
		message = "Texture Loaded";
		break;
	case RE_TYPE::UNDEFINED:
		message = "Undefined file extension";
		break;
	}

	return message;
}

void ResourceManager::CheckRemovedFiles()
{
	FileTree* folder = ModuleFile::FS_GetFileTree(MODELS_PATH);

	for (size_t i = 1; i < folder->files.size(); i += 2)
	{
		std::string full_path = MODELS_PATH + folder->files[i];
		if (!ModuleFile::FS_Exists(ModuleFile::YK_GetMetaInfo(full_path, 1)))
		{
			remove(full_path.c_str());
			remove(ModuleFile::FS_RemoveExtension(full_path).c_str());
		}
	}
	
	folder = ModuleFile::FS_GetFileTree(MESHES_PATH);

	for (size_t i = 1; i < folder->files.size(); i += 2)
	{
		std::string full_path = MESHES_PATH + folder->files[i];
		if (!ModuleFile::FS_Exists(ModuleFile::YK_GetMetaInfo(full_path, 1)))
		{
			remove(full_path.c_str());
			remove(ModuleFile::FS_GetFileName(full_path, false).c_str());
		}
	}

	folder = ModuleFile::FS_GetFileTree(TEXTURES_PATH);

	for (size_t i = 1; i < folder->files.size(); i += 2)
	{
		std::string full_path = TEXTURES_PATH + folder->files[i];
		if (!ModuleFile::FS_Exists(ModuleFile::YK_GetMetaInfo(full_path, 2)))
		{
			remove(full_path.c_str());
			remove(ModuleFile::FS_GetFileName(full_path, false).c_str());
		}
	}

	RELEASE(folder);
}