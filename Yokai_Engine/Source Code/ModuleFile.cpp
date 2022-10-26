#include "ModuleFile.h"
#include "Application.h"
#include "PhysFS/include/physfs.h"
#include "FileTree.hpp"
#include <algorithm>

#pragma comment (lib, "PhysFS/libx86/physfs.lib")

ModuleFile::ModuleFile(bool start_enabled) : Module(start_enabled)
{
	PHYSFS_init(0);

	// Add Write Dir
	if (PHYSFS_setWriteDir(".") == 0) LOG("File System error while creating write dir: %s\n", PHYSFS_getLastError());

	// Add Read Dir
	S_AddPathToFileSystem(".");
	S_AddPathToFileSystem("C:\\");
	S_AddPathToFileSystem("D:\\");

	//S_AddPathToFileSystem("Resources");
}

ModuleFile::~ModuleFile()
{
	PHYSFS_deinit();
}

bool ModuleFile::S_Exists(const std::string file)
{
	return PHYSFS_exists(file.c_str()) != 0;
}

bool ModuleFile::S_MakeDir(const std::string dir)
{
	if (S_IsDirectory(dir) == false)
	{
		PHYSFS_mkdir(dir.c_str());
		return true;
	}
	return false;
}

bool ModuleFile::S_IsDirectory(const std::string file)
{
	return PHYSFS_isDirectory(file.c_str()) != 0;
}

std::string ModuleFile::S_NormalizePath(const std::string path)
{
	std::string ret = path;

	// Remove no necesary path
	int pos = ret.find("C:\\");

	if (pos != std::string::npos) ret.erase(pos, 3);

	pos = ret.find("D:\\");

	if (pos != std::string::npos) ret.erase(pos, 3);

	for (size_t i = 0; i < ret.size(); i++)
	{
		if (ret[i] == '\\') ret[i] = '/';
	}

	return ret;
}

std::string ModuleFile::S_UnNormalizePath(const std::string path)
{
	std::string ret = path;

	for (size_t i = 0; i < ret.size(); i++)
	{
		if (ret[i] == '/') ret[i] = '\\';
	}

	return ret;
}

bool ModuleFile::S_AddPathToFileSystem(const std::string path)
{
	bool ret = false;

	if (PHYSFS_mount(path.c_str(), nullptr, 1) == 0)
	{
		LOG("File System error while adding a path or zip: %s\n", PHYSFS_getLastError());
	}
	else
	{
		ret = true;
	}

	return ret;
}

uint ModuleFile::S_Load(const std::string filePath, char** buffer)
{
	uint byteCount = 0;

	PHYSFS_file* fsFile = PHYSFS_openRead(filePath.c_str());

	do
	{
		if (!fsFile)
		{
			LOG("File System error while opening file %s: %s\n", filePath.c_str(), PHYSFS_getLastError());
			break;
		}

		PHYSFS_sint64 size = PHYSFS_fileLength(fsFile);

		if (size <= 0)
		{
			LOG("File System error while reading from file %s: %s\n", filePath.c_str(), PHYSFS_getLastError());
			break;
		}

		*buffer = new char[(uint)size + 1];

		byteCount = (uint)PHYSFS_readBytes(fsFile, *buffer, size);

		if (byteCount != size)
		{
			LOG("File System error while reading from file %s: %s\n", filePath.c_str(), PHYSFS_getLastError());
			RELEASE_ARRAY(*buffer);
			break;
		}

		//Adding end of file at the end of the buffer. Loading a shader file does not add this for some reason
		(*buffer)[size] = '\0';

	} while (false);

	if (PHYSFS_close(fsFile) == 0) LOG("File System error while closing file %s: %s\n", filePath.c_str(), PHYSFS_getLastError());

	return byteCount;
}

uint ModuleFile::S_Save(const std::string filePath, char* buffer, uint size, bool append)
{
	uint objCount = 0;

	std::string fileName;
	fileName = S_GetFileName(filePath, true);


	bool exists = S_Exists(filePath);

	PHYSFS_file* filehandle = nullptr;
	if (append) filehandle = PHYSFS_openAppend(filePath.c_str());
	else filehandle = PHYSFS_openWrite(filePath.c_str());

	if (filehandle != nullptr)
	{
		objCount = (uint)PHYSFS_writeBytes(filehandle, (const void*)buffer, size);

		if (objCount == size)
		{
			if (exists)
			{
				if (append)
				{
					LOG("FILE SYSTEM: Append %u bytes to file '%s'", objCount, fileName.data());
				}
				else
				{
					LOG("FILE SYSTEM: File '%s' overwritten with %u bytes", fileName.data(), objCount);
				}
			}
			else
			{
				LOG("FILE SYSTEM: New file '%s' created with %u bytes", fileName.data(), objCount);
			}
		}
		else
		{
			LOG("FILE SYSTEM: Could not write to file '%s'. ERROR: %s", fileName.data(), PHYSFS_getLastError());
		}

		if (PHYSFS_close(filehandle) == 0)
		{
			LOG("FILE SYSTEM: Could not close file '%s'. ERROR: %s", fileName.data(), PHYSFS_getLastError());
		}
	}
	else
	{
		LOG("FILE SYSTEM: Could not open file '%s' to write. ERROR: %s", fileName.data(), PHYSFS_getLastError());
	}

	return objCount;
}

uint ModuleFile::S_Copy(const std::string src, std::string des, bool replace)
{
	uint size = 0;
	std::string outputFile;

	std::FILE* filehandle;
	fopen_s(&filehandle, src.c_str(), "rb");

	if (filehandle != nullptr)
	{
		fseek(filehandle, 0, SEEK_END);
		size = ftell(filehandle);
		rewind(filehandle);

		char* buffer = new char[size];
		size = fread(buffer, 1, size, filehandle);
		if (size > 0)
		{
			outputFile = S_GetFileName(src, true);
			outputFile.insert(0, "/");
			outputFile.insert(0, des);

			size = S_Save(outputFile.data(), buffer, size, false);
			if (size > 0)
			{
				LOG("FILE SYSTEM: Successfully copied file '%s' in dir '%s'", src, des);
			}
			else
			{
				LOG("FILE SYSTEM: Could not copy file '%s' in dir '%s'", src, des);
			}
		}
		else
		{
			LOG("FILE SYSTEM: Could not read from file '%s'", src);
		}

		RELEASE_ARRAY(buffer);
		fclose(filehandle);
	}
	else
	{
		LOG("FILE SYSTEM: Could not open file '%s' to read", src);
	}

	return size;
}

FileTree* ModuleFile::S_GetFileTree(std::string path, FileTree* parent)
{
	FileTree* ret = new FileTree(path + "/", S_GetFileName(path), parent);

	char** list = PHYSFS_enumerateFiles(path.c_str());

	for (int i = 0; list[i] != nullptr; i++)
	{
		std::string dirCheck = path + "/" + list[i];

		if (PHYSFS_isDirectory(dirCheck.c_str()) != 0)
		{
			ret->directories.emplace_back(S_GetFileTree(dirCheck, ret));
		}
		else
		{
			ret->files.emplace_back(list[i]);
		}
	}

	return ret;
}

std::string ModuleFile::S_GetFileName(const std::string file, bool getExtension)
{
	std::string fileName = file;

	uint found = fileName.find_last_of("\\");
	if (found != std::string::npos)
		fileName = fileName.substr(found + 1, fileName.size());

	found = fileName.find_last_of("//");
	if (found != std::string::npos)
		fileName = fileName.substr(found + 1, fileName.size());

	if (!getExtension)
	{
		found = fileName.find_last_of(".");
		if (found != std::string::npos)
			fileName = fileName.substr(0, found);
	}

	return fileName;
}

RE_TYPE ModuleFile::S_GetResourceType(const std::string& filename)
{
	std::string fileExtension = filename;
	fileExtension = fileExtension.substr(fileExtension.find_last_of('.') + 1);

	std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);

	//TODO: Add our own file extensions to this checks

	if (fileExtension == "fbx" || fileExtension == "dae") return RE_TYPE::MESH;
	if (fileExtension == "tga" || fileExtension == "png" || fileExtension == "jpg" || fileExtension == "dds") return RE_TYPE::TEXTURE;

	return RE_TYPE::UNDEFINED;
}

std::vector<std::string> ModuleFile::S_GetAllFiles(std::string path)
{
	std::vector<std::string> ret;

	char** list = PHYSFS_enumerateFiles(path.c_str());

	for (int i = 0; list[i] != nullptr; i++)
	{
		std::string dirCheck = path + "/" + list[i];

		if (PHYSFS_isDirectory(dirCheck.c_str()) != 0)
		{
			std::vector<std::string> temp = S_GetAllFiles(dirCheck);
			ret.insert(ret.end(), temp.begin(), temp.end());
		}
		else
		{
			ret.push_back(list[i]);
		}
	}

	return ret;
}