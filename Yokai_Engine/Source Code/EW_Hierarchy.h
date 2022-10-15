#pragma once

#include "EditorWindow.h"

class GameObject;

class EW_Hierarchy : public EditorWindow
{
public:
	EW_Hierarchy();

	~EW_Hierarchy();

	void Update() override;

private:

	int game_width, game_height;
};