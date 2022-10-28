#include "ModuleFile.h"
#include "Application.h"
#include "PhysFS/include/physfs.h"
#include "FileTree.hpp"
#include <algorithm>

#pragma comment (lib, "PhysFS/libx86/physfs.lib")

ModuleFile::ModuleFile(bool start_enabled) : Module(start_enabled)
{
	PHYSFS_init(0);

	if (PHYSFS_setWriteDir(".") == 0) LOG("FileSystem:: %s\n", PHYSFS_getLastError());

	FS_AddPathToFileSystem(".");
	FS_AddPathToFileSystem("C:\\");
	FS_AddPathToFileSystem("D:\\");
}

ModuleFile::~ModuleFile()
{
	PHYSFS_deinit();
}

bool ModuleFile::FS_Exists(const std::string file)
{
	return PHYSFS_exists(file.c_str()) != 0;
}

bool ModuleFile::FS_MakeDir(const std::string dir)
{
	if (FS_IsDirectory(dir) == false)
	{
		PHYSFS_mkdir(dir.c_str());
		return true;
	}
	return false;
}

bool ModuleFile::FS_IsDirectory(const std::string file)
{
	return PHYSFS_isDirectory(file.c_str()) != 0;
}

std::string ModuleFile::FS_NormalizePath(const std::string path)
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

std::string ModuleFile::FS_UnNormalizePath(const std::string path)
{
	std::string ret = path;

	for (size_t i = 0; i < ret.size(); i++)
	{
		if (ret[i] == '/') ret[i] = '\\';
	}

	return ret;
}

bool ModuleFile::FS_AddPathToFileSystem(const std::string path)
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

uint ModuleFile::FS_Load(const std::string filePath, char** buffer)
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

		(*buffer)[size] = '\0';

	} while (false);

	if (PHYSFS_close(fsFile) == 0) LOG("File System error while closing file %s: %s\n", filePath.c_str(), PHYSFS_getLastError());

	return byteCount;
}

uint ModuleFile::FS_Save(const std::string filePath, char* buffer, uint size, bool append)
{
	uint objCount = 0;

	std::string fileName;
	fileName = FS_GetFileName(filePath, true);


	bool exists = FS_Exists(filePath);

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

uint ModuleFile::FS_Copy(const std::string src, std::string des, bool replace)
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
			outputFile = FS_GetFileName(src, true);
			outputFile.insert(0, "/");
			outputFile.insert(0, des);

			size = FS_Save(outputFile.data(), buffer, size, false);
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

FileTree* ModuleFile::FS_GetFileTree(std::string path, FileTree* parent)
{
	FileTree* ret = new FileTree(path + "/", FS_GetFileName(path), parent);

	char** list = PHYSFS_enumerateFiles(path.c_str());

	for (int i = 0; list[i] != nullptr; i++)
	{
		std::string dirCheck = path + "/" + list[i];

		if (PHYSFS_isDirectory(dirCheck.c_str()) != 0)
		{
			ret->directories.emplace_back(FS_GetFileTree(dirCheck, ret));
		}
		else
		{
			ret->files.emplace_back(list[i]);
		}
	}

	return ret;
}

std::string ModuleFile::FS_GetFileName(const std::string file, bool getExtension)
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

RE_TYPE ModuleFile::FS_GetResourceType(const std::string& filename)
{
	std::string fileExtension = filename;
	fileExtension = fileExtension.substr(fileExtension.find_last_of('.') + 1);

	std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);

	//TODO: Add our own file extensions to this checks

	if (fileExtension == "fbx" || fileExtension == "dae") return RE_TYPE::MESH;
	if (fileExtension == "tga" || fileExtension == "png" || fileExtension == "jpg" || fileExtension == "dds") return RE_TYPE::TEXTURE;

	return RE_TYPE::UNDEFINED;
}

std::vector<std::string> ModuleFile::FS_GetAllFiles(std::string path)
{
	std::vector<std::string> ret;

	char** list = PHYSFS_enumerateFiles(path.c_str());

	for (int i = 0; list[i] != nullptr; i++)
	{
		std::string dirCheck = path + "/" + list[i];

		if (PHYSFS_isDirectory(dirCheck.c_str()) != 0)
		{
			std::vector<std::string> temp = FS_GetAllFiles(dirCheck);
			ret.insert(ret.end(), temp.begin(), temp.end());
		}
		else
		{
			ret.push_back(list[i]);
		}
	}

	return ret;
}