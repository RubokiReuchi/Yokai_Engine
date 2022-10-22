#pragma once

#include "Globals.h"
#include "Application.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "IconsFontAwesome.h"

#include <string>

class EditorWindow
{
public:
	EditorWindow();

	virtual ~EditorWindow();

	virtual void Update();

	bool enabled = true;
	std::string window_name = "name";
};