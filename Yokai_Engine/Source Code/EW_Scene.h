#pragma once

#include "EditorWindow.h"

class EW_Scene : public EditorWindow
{
public:
	EW_Scene();

	~EW_Scene();

	void Update() override;

private:

	int game_width, game_height;
};