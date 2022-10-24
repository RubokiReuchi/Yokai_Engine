#pragma once

#include "EditorWindow.h"

class FileTree;

class EW_Project : public EditorWindow
{
public:
	EW_Project();

	~EW_Project();

	void Update() override;

private:
	void DrawTreeNode(const FileTree* node)const;
	void OnDrop(std::string filePath);

	ImGuiTextFilter filter;

	FileTree* fileTree = nullptr;
	FileTree* currentNode = nullptr;
};