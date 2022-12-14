#include "ResourceManager.h"
#include "Application.h"
#include "SceneCamera.h"
#include "ModuleFile.h"
#include "MeshImporter.h"
#include "TextureImporter.h"
#include "FileTree.hpp"
#include "M_Texture.h"

#define VAL(str) #str
#define TOSTRING(str) VAL(str)

std::string ResourceManager::LoadResource(std::string path)
{
	std::string message = "";
	std::string texture_path = "";

	switch (ModuleFile::FS_GetResourceType(path))
	{
	case RE_TYPE::MESH:
		MeshImporter::LoadMesh(path);
		message = "Mesh placed";
		break;
	case RE_TYPE::TEXTURE:
		texture_path = TEXTURES_PATH + ModuleFile::FS_GetFileName(path, false) + ".dds";
		if (M_Texture::usedPaths.find(texture_path) != M_Texture::usedPaths.end())
		{
			GameObject* go = app->camera->sceneCamera.GetGoInMouse();
			if (go != NULL)
			{
				dynamic_cast<C_Material*>(go->GetComponent(Component::TYPE::MATERIAL))->SetTexture(texture_path);
			}
		}
		else
		{
			TextureImporter::LoadTexture(path);
			message = "Texture Loaded";
		}
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
			Console::LogInConsole(ModuleFile::FS_RemoveExtension(full_path) + " delete.");
		}
	}
	
	RELEASE(folder);
	folder = ModuleFile::FS_GetFileTree(MESHES_PATH);

	for (size_t i = 1; i < folder->files.size(); i += 2)
	{
		std::string full_path = MESHES_PATH + folder->files[i];
		if (!ModuleFile::FS_Exists(ModuleFile::YK_GetMetaInfo(full_path, 1)))
		{
			remove(full_path.c_str());
			remove(ModuleFile::FS_RemoveExtension(full_path).c_str());
			Console::LogInConsole(ModuleFile::FS_RemoveExtension(full_path) + " delete.");
		}
	}

	RELEASE(folder);
	folder = ModuleFile::FS_GetFileTree(TEXTURES_PATH);

	for (size_t i = 1; i < folder->files.size(); i += 2)
	{
		std::string full_path = TEXTURES_PATH + folder->files[i];
		if (!ModuleFile::FS_Exists(ModuleFile::YK_GetMetaInfo(full_path, 1)))
		{
			remove(full_path.c_str());
			remove(ModuleFile::FS_RemoveExtension(full_path).c_str());
			Console::LogInConsole(ModuleFile::FS_RemoveExtension(full_path) + " delete.");
		}
	}

	RELEASE(folder);
}

void ResourceManager::CheckModifiedFiles()
{
	FileTree* folder = ModuleFile::FS_GetFileTree(MODELS_PATH);

	for (size_t i = 1; i < folder->files.size(); i += 2)
	{
		std::string full_path = MODELS_PATH + folder->files[i];
		if (ModuleFile::FS_GetLastModifyDate(ModuleFile::YK_GetMetaInfo(full_path, 1)) != ModuleFile::YK_GetMetaInfo(full_path, 2))
		{
			remove(full_path.c_str());
			remove(ModuleFile::FS_RemoveExtension(full_path).c_str());
			Console::LogInConsole(ModuleFile::FS_RemoveExtension(full_path) + " delete.");
		}
	}

	RELEASE(folder);
	folder = ModuleFile::FS_GetFileTree(MESHES_PATH);

	for (size_t i = 1; i < folder->files.size(); i += 2)
	{
		std::string full_path = MESHES_PATH + folder->files[i];
		if (ModuleFile::FS_GetLastModifyDate(ModuleFile::YK_GetMetaInfo(full_path, 1)) != ModuleFile::YK_GetMetaInfo(full_path, 2))
		{
			remove(full_path.c_str());
			remove(ModuleFile::FS_RemoveExtension(full_path).c_str());
			Console::LogInConsole(ModuleFile::FS_RemoveExtension(full_path) + " delete.");
		}
	}

	RELEASE(folder);
	folder = ModuleFile::FS_GetFileTree(TEXTURES_PATH);

	for (size_t i = 1; i < folder->files.size(); i += 2)
	{
		std::string full_path = TEXTURES_PATH + folder->files[i];
		if (ModuleFile::FS_GetLastModifyDate(ModuleFile::YK_GetMetaInfo(full_path, 1)) != ModuleFile::YK_GetMetaInfo(full_path, 2))
		{
			remove(full_path.c_str());
			remove(ModuleFile::FS_RemoveExtension(full_path).c_str());
			Console::LogInConsole(ModuleFile::FS_RemoveExtension(full_path) + " delete.");
		}
	}

	RELEASE(folder);
}

std::string ResourceManager::BuildPath(std::string path)//system("\"C:\\Users\\jayor\\Desktop\\hola.text\"");
{
	std::string new_path = "";
	std::string aux;
	size_t n = std::count(path.begin(), path.end(), '/');
	size_t npos;

	for (size_t i = 0; i < n; i++)
	{
		npos = path.find_first_of("/");
		aux = path;
		new_path += aux.erase(npos, path.length()) + "\\";
	}
	npos = path.find_last_of("/");
	aux = path;
	new_path += aux.erase(0, npos + 1);

	return new_path;
}

void ResourceManager::OpenResource(std::string path)
{
	std::string open_path = BuildPath(path);
	ShellExecute(NULL, "open", open_path.c_str(), NULL, NULL, SW_HIDE);
}

void ResourceManager::DeleteResource(std::string path)
{
	remove(path.c_str());
	Console::LogInConsole(path + " deleted.");
	CheckRemovedFiles();
}