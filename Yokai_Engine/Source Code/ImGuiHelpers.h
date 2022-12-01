#pragma once

#include "Globals.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

#include <string>

namespace ImGuiH
{
	IMGUI_API bool  InputText(const char* label, std::string* str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
	bool CheckMouseInPopUp(ImVec2 ori, ImVec2 popup_size);
	bool ButtonAlignOnLine(const char* label, float alignment = 0.5f); // 0 --> align left, 0.5 --> align center, 1 --> align right
	void TextAlignOnLine(const char* label, float alignment = 0.5f); // 0 --> align left, 0.5 --> align center, 1 --> align right
}