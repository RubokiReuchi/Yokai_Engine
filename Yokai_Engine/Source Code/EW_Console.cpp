#include "EW_Console.h"
#include "Console.h"

EW_Console::EW_Console()
{
	window_name = "Console";
	enabled = true;

	Console::LogInConsole("Window Console Start");
	Console::LogInConsole("PHYSFS::Inizializated");
	Console::LogInConsole("ImGui::Inizializated");
	Console::LogInConsole("DevIL::Inizializated");
}

EW_Console::~EW_Console()
{
	
}

void EW_Console::Update()
{
	// Console
	ImGui::Begin(window_name.c_str(), &enabled, ImGuiWindowFlags_NoCollapse);
	if (ImGui::SmallButton("Save"))
	{
		Console::SaveConsoleLog();
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("Clear"))
	{
		Console::ClearConsoleLog();
	}

	ImGui::Separator();

	const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);

	Console::DrawConsoleLog();

	ImGui::EndChild();
	ImGui::End();
}