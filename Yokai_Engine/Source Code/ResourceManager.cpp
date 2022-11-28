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

}