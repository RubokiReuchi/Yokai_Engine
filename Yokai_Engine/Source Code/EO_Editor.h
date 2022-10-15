#pragma once

#include "EO_Base.h"
#include "EditorWindow.h"

enum class EW_TYPE
{
	HIERARCHY,
	SCENE,
	ABOUT,
	INSPECTOR,
	PERFORMANCE,
	PROJECT,
	CONSOLE,
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

private:

	EditorWindow* editor_windows[(uint)EW_TYPE::NUM_EW_TYPE] = { nullptr };

	bool SetMenuBar();
};