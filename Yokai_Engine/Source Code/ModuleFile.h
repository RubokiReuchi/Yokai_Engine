#pragma once

#include "Module.h"
#include "Globals.h"

#include <string>

class ModuleFile : public Module
{
public:
	ModuleFile(bool start_enabled = true);

	~ModuleFile();

	// Static functions
	static bool S_Exists(const std::string file);

	static bool S_MakeDir(const std::string dir);

	static bool S_IsDirectory(const std::string file);

	// can be path or zip
	static bool S_AddPathToFileSystem(const std::string path);

	/// <summary>
	/// Read directory => /Output
	/// </summary>
	/// <param name="filePath">: path/file.ext</param>
	/// <param name="buffer">: buffer where store binary file info</param>
	/// <returns></returns>
	static uint S_Load(const std::string filePath, char** buffer);

	static uint S_Save(const std::string filePath, char* buffer, uint size, bool append);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="src">: PATH WITH NAME!!!</param>
	/// <param name="des">: JUST PATH!!!</param>
	/// <param name="replace">: Replace file if is aldready exist</param>
	/// <returns></returns>
	static uint S_Copy(const std::string src, const std::string des, bool replace = true);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="file"></param>
	/// <param name="getExtention">: If you want to get the extension</param>
	/// <returns></returns>
	static std::string GetFileName(const std::string file, bool getExtension = true);
};