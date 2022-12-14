#pragma once

#include "EditorWindow.h"

#include <vector>

class GameObject;
class EO_Editor;

class EW_Hierarchy : public EditorWindow
{
public:
	EW_Hierarchy();

	~EW_Hierarchy();

	void Update() override;

	void DrawGameObjecTree(GameObject* gameObject, bool onlyChildren = false);
	void DrawGameObject(GameObject* gameObject, int iteration);

	GameObject* GetDraggingGO()
	{
		return draggingGameObject;
	}
	void NullDraggingGO()
	{
		draggingGameObject = nullptr;
	}

private:
	static const int numSubmenus = 2;
	static const int numPrimitives = 5;

	std::map<uint, GameObject*>* gameObjectsReference = nullptr;
	EO_Editor* editor = nullptr;

	GameObject* draggingGameObject = nullptr;
	bool optionsOpen = false; //options popUp

	ImGuiTreeNodeFlags base_flags;

	bool popUpOpen = false; //create popUp
	ImVec2 ori;

	void SetVisibleOnEditor(GameObject* first_game_object, GameObject* game_object, bool visible_on_editor, bool it_one);
};