#pragma once

#include "EditorWindow.h"

class GameObject;
class EO_Editor;

class EW_Hierarchy : public EditorWindow
{
public:
	EW_Hierarchy();

	~EW_Hierarchy();

	void Update() override;

	void DrawGameObjectChildren(GameObject* gameObject, bool onlyChildren = false);
	void ProcessGameObject(GameObject* gameObject, int iteration);

private:
	static const int numPrimitives = 5;

	std::map<uint, GameObject*>* gameObjectsReference = nullptr;
	EO_Editor* editor = nullptr;

	GameObject* draggingGameObject = nullptr;
	GameObject* rightClickedGameObject = nullptr;

	ImGuiTreeNodeFlags base_flags;

	bool popUpOpen = false;

	void CheckMouseInPopUp();
	ImVec2 ori;

	void UpdateVisibleOnEditor(GameObject* gameObject, bool visible_on_editor);
};