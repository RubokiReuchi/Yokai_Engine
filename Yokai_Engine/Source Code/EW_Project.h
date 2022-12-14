#pragma once

#include "EditorWindow.h"

class FileTree;

class EW_Project : public EditorWindow
{
public:
	EW_Project();

	~EW_Project();

	void Update() override;

	std::string GetCurrentNodePath();

private:
	void DrawFileBox(FileTree* folder);

	ImGuiTextFilter filter;

	FileTree* fileTree = nullptr;
	FileTree* currentNode = nullptr;

	std::vector<std::string> allFiles;

	bool open_file = false;
	bool comfirm_popUp = false;
	ImVec2 ori;
	std::string file_to_delete = "";
};