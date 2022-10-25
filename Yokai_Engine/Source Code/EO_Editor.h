#pragma once

#include "EO_Base.h"
#include "EditorWindow.h"
#include "EW_Project.h"

class GameObject;
class EW_Project;

enum class EW_TYPE
{
	HIERARCHY,
	ABOUT,
	GAME,
	SCENE,
	PERFORMANCE,
	INSPECTOR,
	CONSOLE,
	PROJECT,
	NUM_EW_TYPE, // number of EditorWindows types
};

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

	void SetSelectedGameObject(GameObject* go)
	{
		selected_go = go;
	}
	GameObject* GetSelectedGameObject()
	{
		return selected_go;
	}

	std::string dd_file = ""; // drag and drop
	EW_Project* GetProjectWindow()
	{
		return dynamic_cast<EW_Project*>(editor_windows[(uint)EW_TYPE::PROJECT]);
	}

private:

	EditorWindow* editor_windows[(uint)EW_TYPE::NUM_EW_TYPE] = { nullptr };

	GameObject* selected_go = nullptr;

	bool SetMenuBar();
};