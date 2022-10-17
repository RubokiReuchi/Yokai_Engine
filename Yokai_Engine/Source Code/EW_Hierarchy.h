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

	void DrawGameObjectChildren(GameObject* gameObject, int layer);

private:
	std::map<uint, GameObject*>* gameObjectsReference = nullptr;
	EO_Editor* editor = nullptr;

	GameObject* draggingGameObject = nullptr;

	ImGuiTreeNodeFlags base_flags;
};