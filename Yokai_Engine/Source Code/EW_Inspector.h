#pragma once

#include "EditorWindow.h"

class GameObject;
class EO_Editor;

class EW_Inspector : public EditorWindow
{
public:
	EW_Inspector();

	~EW_Inspector();

	void Update() override;

private:

	EO_Editor* editor = nullptr;
	GameObject* selectGameobject = nullptr;
	friend class EO_Editor;
};