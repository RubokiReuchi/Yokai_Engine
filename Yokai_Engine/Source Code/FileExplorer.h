#pragma once

#include "Globals.h"
#include "ImGuiHelpers.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "IconsFontAwesome.h"

#include <string>

class FileTree;

struct Explorer
{
	enum class TYPE
	{
		UNINICIALIZED,
		SAVE,
		LOAD
	};

	enum class OPTION
	{
		SCENE,
		SCRIPT
	};

	std::string name = "name";
	TYPE type = TYPE::UNINICIALIZED;
	std::string path = "";
	bool opened = false;
	std::string file_name = "";
	OPTION option = OPTION::SCENE;
};

class FileExplorer
{
public:
	~FileExplorer();

	void OpenExplorer(std::string name, Explorer::TYPE type, std::string path);
	void DrawExplorer();

private:
	void InitExplorer(std::string name, Explorer::TYPE type, std::string path);
	void ClearExplorer();
	void CloseExplorer();

	void DrawFiles(std::string& file_name);

	void CkeckDirectory(std::string file_name);

	Explorer explorer;

	FileTree* directory = nullptr;

	bool comfirm_popUp = false;
	ImVec2 ori;

};