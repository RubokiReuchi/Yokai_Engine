#include "EW_Console.h"
#include "Console.h"

EW_Console::EW_Console()
{
	window_name = "Console";
	enabled = true;

	Console::S_Init();

	Console::S_Log("Window Console Start");

	Console::S_Log("PHYSFS::Inizializated");

	Console::S_Log("ImGui::Inizializated");

	Console::S_Log("DevIL::Inizializated");
}

EW_Console::~EW_Console()
{
	Console::S_Close();
}

void EW_Console::Update()
{
	// Console
	static int num = 0;
	ImGui::Begin(window_name.c_str(), &enabled, ImGuiWindowFlags_NoCollapse);
	if (ImGui::SmallButton("Save Log"))
	{
		Console::S_SaveLog();
	}
	ImGui::SameLine();

	if (ImGui::SmallButton("Resume"))
	{
		resume = !resume;
	}
	ImGui::SameLine();

	if (ImGui::SmallButton("Clear"))
	{
		Console::S_ClearLog();
	}
	ImGui::SameLine();

	ImGui::Text("Total Logs: %s", Console::S_GetLogCounts());

	ImGui::Separator();

	// Reserve enough left-over height for 1 separator + 1 input text
	const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
	if (!resume)
	{
		std::string* buffer = nullptr;

		uint console_size = Console::S_GetLog(&buffer);

		for (size_t i = 0; i < console_size; buffer++, i++)
		{
			ImGui::Text((*buffer).c_str());
		}
	}
	else
	{
		auto logs = Console::S_GetCollapseLog();

		for (auto& log : logs)
		{
			ImGui::Text((log.first + "\tcount: " + std::to_string(log.second)).c_str());
		}
	}
	ImGui::EndChild();
	ImGui::End();
}