#pragma once

#include "EditorWindow.h"

class GameObject;

class EW_Inspector : public EditorWindow
{
public:
	EW_Inspector();

	~EW_Inspector();

	void Update() override;

private:

	GameObject* selectGameobject = nullptr;
};