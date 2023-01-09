#pragma once

#include "EO_Base.h"
#include "EditorWindow.h"
#include "EW_Hierarchy.h"
#include "EW_Project.h"
#include "EW_Game.h"
#include "EW_Console.h"
#include "FileExplorer.h"

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
	BLUEPRINT,
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

	void SetSelectedGameObject(GameObject* go);
	GameObject* GetSelectedGameObject()
	{
		return selected_go;
	}

	FileExplorer file_explorer;

	// Fonts
	ImFont* default_font;
	ImFont* arial_font_30;

	// drag and drop
	std::string dd_file = "";
	std::string message = "";
	EW_Project* GetProjectWindow()
	{
		return dynamic_cast<EW_Project*>(editor_windows[(uint)EW_TYPE::PROJECT]);
	}

	EW_Hierarchy* GetHierarchyWindow()
	{
		return dynamic_cast<EW_Hierarchy*>(editor_windows[(uint)EW_TYPE::HIERARCHY]);
	}

	EW_Game* GetGameWindow()
	{
		return dynamic_cast<EW_Game*>(editor_windows[(uint)EW_TYPE::GAME]);
	}

private:

	EditorWindow* editor_windows[(uint)EW_TYPE::NUM_EW_TYPE] = { nullptr };

	GameObject* selected_go = nullptr;

	bool SetMenuBar();

	void SetOutline(GameObject* selected_game_object, GameObject* game_object);
};