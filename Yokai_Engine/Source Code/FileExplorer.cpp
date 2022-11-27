#include "FileExplorer.h"
#include "ModuleFile.h"
#include "Console.h"
#include "Serialization.h"
#include "FileTree.hpp"

void FileExplorer::InitExplorer(std::string name, Explorer::TYPE type, std::string path)
{
	explorer.name = name;
	explorer.type = type;
	explorer.path = path;
}

void FileExplorer::ClearExplorer()
{
	explorer.name = "name";
	explorer.type = Explorer::TYPE::UNINICIALIZED;
	explorer.path = "";
}

void FileExplorer::OpenExplorer(std::string name, Explorer::TYPE type, std::string path)
{
	if (!explorer.opened)
	{
		InitExplorer(name, type, path);
		explorer.opened = true;

		directory = ModuleFile::FS_GetFileTree(SCENES_PATH);
	}
	else Console::LogInConsole("Explorer " + explorer.name + " already open.");
}

void FileExplorer::CloseExplorer()
{
	if (explorer.opened)
	{
		explorer.opened = false;
		RELEASE(directory);
		ClearExplorer();
	}
	else Console::LogInConsole("Explorer " + explorer.name + " already close.");
}

void FileExplorer::DrawExplorer()
{
	if (!explorer.opened) return;

	std::string explorer_confirm;
	switch (explorer.type)
	{
	case Explorer::TYPE::UNINICIALIZED:
		Console::LogInConsole("Explorer " + explorer.name + " can't draw because is uninicialized.");
		return;
		break;
	case Explorer::TYPE::SAVE:
		explorer_confirm = "Save Scene";
		break;
	case Explorer::TYPE::LOAD:
		explorer_confirm = "Load Scene";
		break;
	}

	ImGui::Begin(explorer.name.c_str(), &explorer.opened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking);
	ImGui::Text("Path:");
	ImVec2 start = ImGui::GetItemRectMin();
	ImGui::SameLine();
	ImGui::Text(explorer.path.c_str());
	ImVec2 end = ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowWidth(), ImGui::GetItemRectMax().y);
	start.y = end.y;
	ImGui::GetWindowDrawList()->AddLine(start, end, ImColor(0.824f, 0.824f, 0.824f, 1.0f), 1.0f);
	DrawFiles(explorer.scene_name);
	ImGui::Text("File Name:");
	ImGui::SameLine();
	ImGuiH::InputText("##Name", &explorer.scene_name);
	ImGui::SameLine();
	if (ImGuiH::ButtonAlignOnLine(explorer_confirm.c_str(), 1.0f))
	{
		CkeckDirectory(explorer.scene_name);
	}
	ImGui::End();
}

void FileExplorer::DrawFiles(std::string& scene_name)
{
	std::vector<std::string> file_paths = ModuleFile::FS_RemoveExtensions(directory->files);
	ImGui::ListBoxHeader("##Files in scenes", ImVec2(ImGui::GetWindowWidth(), 0));
	for (auto& file : file_paths)
	{
		std::string& item_name = file;
		if (ImGui::Selectable(item_name.c_str()))
		{
			scene_name = file;
		}
	}
	ImGui::ListBoxFooter();
}

void FileExplorer::CkeckDirectory(std::string scene_name)
{
	if (explorer.type == Explorer::TYPE::SAVE)
	{
		if (std::find(directory->files.begin(), directory->files.end(), scene_name) == directory->files.end()) // file not found
		{
			Serialization::YK_SaveScene(scene_name);
			CloseExplorer();
		}
	}
}