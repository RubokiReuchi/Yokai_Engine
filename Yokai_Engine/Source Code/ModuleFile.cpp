#include "ModuleFile.h"
#include "Application.h"
#include "ResourceManager.h"
#include "PhysFS/include/physfs.h"
#include "FileTree.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>

#pragma comment (lib, "PhysFS/libx86/physfs.lib")

template <typename T>
std::time_t to_time_t(T t)
{
	using namespace std::chrono;
	auto sctp = time_point_cast<system_clock::duration>(t - T::clock::now() + system_clock::now());
	return system_clock::to_time_t(sctp);
}

ModuleFile::ModuleFile(bool start_enabled) : Module(start_enabled)
{
	PHYSFS_init(0);

	if (PHYSFS_setWriteDir(".") == 0) LOG("FileSystem:: %s\n", PHYSFS_getLastError());

	FS_AddPathToFileSystem(".");
	FS_AddPathToFileSystem("C:\\");
	FS_AddPathToFileSystem("D:\\");

	YK_CreateLibrary();
	ResourceManager::CheckRemovedFiles();
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

	if (fileExtension == "fbx") return RE_TYPE::MESH;
	if (fileExtension == "png" || fileExtension == "jpg") return RE_TYPE::TEXTURE;

	return RE_TYPE::UNDEFINED;
}

std::string ModuleFile::FS_GetExtension(const std::string& file)
{
	std::string fileExtension = file;
	fileExtension = fileExtension.substr(fileExtension.find_last_of('.'));

	return fileExtension;
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
			std::string s = path + "/" + list[i];
			ret.push_back(s);
		}
	}
	
	return ret;
}

std::vector<std::string> ModuleFile::FS_RemoveExtensions(std::vector<std::string> paths)
{
	std::vector<std::string> new_paths;
	for (size_t i = 0; i < paths.size(); i++)
	{
		size_t pos = paths[i].find_last_of(".");
		new_paths.push_back(paths[i].substr(0, pos));
	}

	return new_paths;
}

std::string ModuleFile::FS_RemoveExtension(std::string path)
{
	std::string new_path;
	size_t pos = path.find_last_of(".");
	new_path = path.substr(0, pos);

	return new_path;
}

std::string ModuleFile::FS_GetLastModifyDate(std::string path)
{
	std::filesystem::file_time_type date = std::filesystem::last_write_time(path);
	std::time_t aux = to_time_t(date);
	std::tm* gmt = std::gmtime(&aux);
	std::stringstream buffer;
	buffer << std::put_time(gmt, "%A, %d %B %Y %H:%M");
	std::string formattedFileTime = buffer.str();
	return formattedFileTime;
}

void ModuleFile::YK_CreateLibrary()
{
	if (!FS_IsDirectory(LIBRARY_PATH)) PHYSFS_mkdir(LIBRARY_PATH);
	if (!FS_IsDirectory(MODELS_PATH)) PHYSFS_mkdir(MODELS_PATH);
	if (!FS_IsDirectory(MESHES_PATH)) PHYSFS_mkdir(MESHES_PATH);
	if (!FS_IsDirectory(TEXTURES_PATH)) PHYSFS_mkdir(TEXTURES_PATH);
	if (!FS_IsDirectory(MATERIALS_PATH)) PHYSFS_mkdir(MATERIALS_PATH);
	if (!FS_IsDirectory(SCENES_PATH)) PHYSFS_mkdir(SCENES_PATH);
	if (!FS_IsDirectory(SCRIPTS_PATH)) PHYSFS_mkdir(SCRIPTS_PATH);
}

void ModuleFile::YK_SaveModel(std::string path, std::vector<std::string> children_paths)
{
	std::ofstream file(path, std::ofstream::out);
	if (file.is_open())
	{
		for (size_t i = 0; i < children_paths.size(); i++)
		{
			file << children_paths[i] << "\n";
		}
		file.close();
	}
	else
	{
		LOG("Error saving model.");
	}
}

std::vector<std::string> ModuleFile::YK_LoadModel(std::string path)
{
	std::vector<std::string> ret;
	std::string line;

	std::fstream file(path, std::ofstream::in);
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			ret.push_back(line);
		}
	}
	else
	{
		LOG("Error loading model.");
	}

	return ret;
}

char* ModuleFile::YK_SaveMesh(uint& size, std::vector<VertexInfo> vertices, std::vector<uint> indices)
{
	size_t num_indices = indices.size();
	size_t num_vertices = vertices.size();
	uint ranges[2] = { num_indices, num_vertices };
	size = sizeof(ranges) + (sizeof(uint) * num_indices) + (sizeof(VertexInfo) * num_vertices);

	char* buffer = new char[size];
	char* cursor = buffer;

	uint bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);
	cursor += bytes;

	bytes = sizeof(uint) * num_indices;
	memcpy(cursor, &indices[0], bytes);
	cursor += bytes;

	bytes = sizeof(VertexInfo) * num_vertices;
	memcpy(cursor, &vertices[0], bytes);
	cursor += bytes;

	return buffer;
}

void ModuleFile::YK_SaveMetaData(std::string file_name, std::string file_reference)
{
	file_name += ".meta";
	std::ofstream matafile(file_name, std::ofstream::out);
	if (matafile.is_open())
	{
		// line 0, path of "parent file"
		matafile << file_reference << "\n";

		// line 1, last file modification
		matafile << FS_GetLastModifyDate(file_reference) << "\n";
		matafile.close();

		// set meta file in hidden
		DWORD attributes = GetFileAttributes(file_name.c_str());
		SetFileAttributes(file_name.c_str(), attributes + FILE_ATTRIBUTE_HIDDEN);
	}
	else
	{
		LOG("Error saving meta data.");
	}
}

std::string ModuleFile::YK_GetMetaInfo(std::string path, int line)
{
	std::string ret;

	// unhide to allow reading
	DWORD attributes = GetFileAttributes(path.c_str());
	SetFileAttributes(path.c_str(), attributes - FILE_ATTRIBUTE_HIDDEN);

	std::fstream metafile(path, std::ofstream::in);
	if (metafile.is_open())
	{
		for (int i = 0; i < line; i++)
		{
			std::getline(metafile, ret);
		}
	}
	else
	{
		LOG("Error reading meta data.");
	}

	// hide again
	attributes = GetFileAttributes(path.c_str());
	SetFileAttributes(path.c_str(), attributes + FILE_ATTRIBUTE_HIDDEN);

	return ret;
}