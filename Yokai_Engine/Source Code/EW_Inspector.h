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

	void SetVisible(GameObject* selected_game_object, GameObject* game_object,  bool visible, bool it_one);

	bool popUpOpen = false;
	ImVec2 ori;

	ImGuiTextFilter filter;
	static const int numComponents = (int)Component::TYPE::NUM_OF_COMPS - 1;
	void DisplayComponents();
};