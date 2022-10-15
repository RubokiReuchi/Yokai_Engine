#pragma once

#include "EditorWindow.h"

class EW_About : public EditorWindow
{
public:
	EW_About();

	~EW_About();

	void Update() override;

	void OsOpenInShell(const char* path);
};