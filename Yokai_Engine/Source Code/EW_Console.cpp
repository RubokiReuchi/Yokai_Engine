#include "EW_Console.h"
#include "Console.h"

EW_Console::EW_Console()
{
	window_name = "Console";
	enabled = true;

	Console::InitConsole();

	Console::LogInConsole("Window Console Start");

	Console::LogInConsole("PHYSFS::Inizializated");

	Console::LogInConsole("ImGui::Inizializated");

	Console::LogInConsole("DevIL::Inizializated");
}

EW_Console::~EW_Console()
{
	Console::CloseConsole();
}

void EW_Console::Update()
{
	// Console
	static int num = 0;
	ImGui::Begin(window_name.c_str(), &enabled, ImGuiWindowFlags_NoCollapse);
	if (ImGui::SmallButton("Clear"))
	{
		Console::ClearConsole();
	}
	ImGui::SameLine();

	ImGui::Text("Total Logs: %s", Console::GetLogCounts());

	ImGui::Separator();

	const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);

	std::string* buffer = nullptr;
	uint console_size = Console::GetConsoleLog(&buffer);
	for (size_t i = 0; i < console_size; buffer++, i++)
	{
		ImGui::Text((*buffer).c_str());
	}

	ImGui::EndChild();
	ImGui::End();
}