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
	void DrawTreeNode(const FileTree* node)const;

	ImGuiTextFilter filter;

	FileTree* fileTree = nullptr;
	FileTree* currentNode = nullptr;

	std::vector<std::string> allFiles;
};