#include "FileExplorer.h"
#include "Application.h"
#include "ModuleFile.h"
#include "Console.h"
#include "Serialization.h"
#include "FileTree.hpp"

FileExplorer::~FileExplorer()
{
	RELEASE(directory);
}

void FileExplorer::InitExplorer(std::string name, Explorer::TYPE type, std::string path)
{
	explorer.name = name;
	explorer.type = type;
	explorer.path = path;
	explorer.scene_name = "";
}

void FileExplorer::ClearExplorer()
{
	explorer.name = "name";
	explorer.type = Explorer::TYPE::UNINICIALIZED;
	explorer.path = "";
	explorer.scene_name = "";
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

	ImGuiInputTextFlags_ textinput_flags = ImGuiInputTextFlags_None;
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
	if (explorer.type == Explorer::TYPE::LOAD) textinput_flags = ImGuiInputTextFlags_ReadOnly;
	ImGuiH::InputText("##Name", &explorer.scene_name, textinput_flags);
	
	ImGui::SameLine();
	if (ImGuiH::ButtonAlignOnLine(explorer_confirm.c_str(), 1.0f))
	{
		CkeckDirectory(explorer.scene_name);
	}

	// confirm popUp
	if (comfirm_popUp)
	{
		ImGui::SetNextWindowSize(ImVec2(150.0f, 75.0f));
		if (ImGui::BeginPopup("Confirm Save"))
		{
			ImGuiH::TextAlignOnLine("Overwrite file?");
			if (ImGuiH::ButtonAlignOnLine("Confirm"))
			{
				Serialization::YK_SaveScene(explorer.scene_name);
				ImGui::CloseCurrentPopup();
				CloseExplorer();
			}
			if (ImGuiH::ButtonAlignOnLine("Cancel"))
			{
				comfirm_popUp = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		if (!ImGuiH::CheckMouseInPopUp(ori, ImVec2(150.0f, 75.0f)))
		{
			comfirm_popUp = false;
			ImGui::CloseCurrentPopup();
		}
	}
	
	ImGui::End();
}

void FileExplorer::DrawFiles(std::string& scene_name)
{
	std::vector<std::string> file_paths = ModuleFile::FS_RemoveExtensions(directory->files);
	ImGui::ListBoxHeader("##Files in scenes", ImVec2(ImGui::GetWindowWidth(), 0));
	for (auto& file : file_paths)
	{
		if (file != app->GetSceneName())
		{
			std::string& item_name = file;
			if (ImGui::Selectable(item_name.c_str()))
			{
				scene_name = file;
			}
		}
	}
	ImGui::ListBoxFooter();
}

void FileExplorer::CkeckDirectory(std::string scene_name)
{
	if (explorer.type == Explorer::TYPE::SAVE)
	{
		if (scene_name != "")
		{
			if (std::find(directory->files.begin(), directory->files.end(), scene_name + ".ykscene") == directory->files.end()) // file not found
			{
				Serialization::YK_SaveScene(scene_name);
				app->SetSceneName(scene_name);
				CloseExplorer();
			}
			else
			{
				ImGui::OpenPopup("Confirm Save");
				comfirm_popUp = true;
				ori = ImGui::GetMousePosOnOpeningCurrentPopup();
			}
		}
	}
	else if (explorer.type == Explorer::TYPE::LOAD)
	{
		if (scene_name != "")
		{
			Serialization::YK_LoadScene(scene_name);
			app->SetSceneName(scene_name);
			CloseExplorer();
		}
	}
}