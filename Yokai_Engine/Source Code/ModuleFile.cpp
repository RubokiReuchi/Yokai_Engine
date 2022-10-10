#include "ModuleFile.h"/*
#include "Application.h"
#include "PhysFS/include/physfs.h"

ModuleFile::ModuleFile(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	PHYSFS_init(0);

	// Add Write Dir
	if (PHYSFS_setWriteDir(".") == 0) LOG("File System error while creating write dir: %s\n", PHYSFS_getLastError());

	// Add Read Dir
	S_AddPathToFileSystem(".");

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

bool ModuleFile::S_AddPathToFileSystem(const std::string path)
{
	bool ret = false;

	if (PHYSFS_mount(path.c_str(), nullptr, 1) == 0)
	{
		LOG("File System error while adding a path or zip: %s\n", PHYSFS_getLastError());
	}
	else
		ret = true;

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

		*buffer = new char[size + 1];

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
	uint byteCount = 0;

	bool exist = S_Exists(filePath);

	PHYSFS_file* des = nullptr;

	do
	{
		if (append)	des = PHYSFS_openAppend(filePath.c_str());
		else des = PHYSFS_openWrite(filePath.c_str());

		if (!des)
		{
			LOG("FILE SYSTEM: Could not open file '%s' to write. ERROR: %s", filePath.c_str(), PHYSFS_getLastError());
			break;
		}

		byteCount = PHYSFS_writeBytes(des, (const void*)buffer, size);

		if (byteCount != size)
		{
			LOG("FILE SYSTEM: Could not write to file '%s'. ERROR: %s", filePath.c_str(), PHYSFS_getLastError());
			break;
		}

		if (!exist)
		{
			LOG("FILE SYSTEM: New file '%s' created with %u bytes", filePath.c_str(), byteCount);
			break;
		}

		if (append)
		{
			LOG("FILE SYSTEM: Append %u bytes to file '%s'", byteCount, filePath.c_str());

			break;
		}

		LOG("FILE SYSTEM: File '%s' overwritten with %u bytes", filePath.c_str(), byteCount);

	} while (false);

	if (PHYSFS_close(des) == 0) LOG("FILE SYSTEM: Could not close file '%s'. ERROR: %s", filePath.c_str(), PHYSFS_getLastError());

	return byteCount;
}

uint ModuleFile::S_Copy(const std::string src, std::string des, bool replace)
{
	uint byteCount = 0;

	std::string fileName = GetFileName(src, true);

	des += fileName;

	do
	{
		if (S_Exists(des) && !replace)
		{
			LOG("FILE SYSTEM: the file you want to copy is already exist and you don't want to replace this: '%s'", src.c_str());
			break;
		}

		char* buffer = nullptr;

		uint srcSize = S_Load(src, &buffer);

		if (srcSize <= 0)
		{
			LOG("FILE SYSTEM: Could not read from file '%s'", src.c_str());
			break;
		}

		uint desSize = S_Save(des, buffer, srcSize, false);

		if (desSize <= 0)
		{
			LOG("FILE SYSTEM: Could not save file '%s'", src.c_str());
			break;
		}

		LOG("FILE SYSTEM: Successfully copied source file: '%s' to the destination file: '%s'", src.c_str(), des.c_str());

	} while (false);

	return byteCount;
}

std::string ModuleFile::GetFileName(const std::string file, bool getExtension)
{
	uint pos = file.find_last_of("/");

	std::string name = "";

	if (pos != std::string::npos) std::string name = file.substr(pos + 1, file.size() - 1);
	else name = file;

	if (!getExtension)
	{
		uint ePos = name.find(".");
		if (pos != std::string::npos) name = name.substr(0, ePos);
	}

	return name;
}*/