#include "EW_Console.h"

EW_Console::EW_Console()
{
	window_name = "Console";
	enabled = true;
}

EW_Console::~EW_Console()
{
}

void EW_Console::Update()
{
	// Console
	ImGui::Begin(window_name.c_str(), &enabled, /*ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | */ImGuiWindowFlags_NoCollapse);
	
	ImGui::End();
}