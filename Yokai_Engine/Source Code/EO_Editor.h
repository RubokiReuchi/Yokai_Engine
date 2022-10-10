#pragma once

#include "EO_Base.h"

class EO_Editor :public EO_Base
{
public:
	EO_Editor();
	~EO_Editor();

	void Start();

	void PreUpdate();
	void Update();
	void PostUpdate();

	void CleanUp();

private:

	bool SetMenuBar();

	bool show_about = false;
	int game_width, game_height;

	void OsOpenInShell(const char* path);
};