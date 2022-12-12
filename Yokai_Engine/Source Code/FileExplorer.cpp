#include "FileExplorer.h"
#include "Application.h"
#include "ModuleFile.h"
#include "Console.h"
#include "Serialization.h"
#include "EO_Editor.h"
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
	explorer.file_name = "";
}

void FileExplorer::ClearExplorer()
{
	explorer.name = "name";
	explorer.type = Explorer::TYPE::UNINICIALIZED;
	explorer.path = "";
	explorer.file_name = "";
}

void FileExplorer::OpenExplorer(std::string name, Explorer::TYPE type, std::string path)
{
	if (!explorer.opened)
	{
		InitExplorer(name, type, path);
		explorer.opened = true;

		directory = ModuleFile::FS_GetFileTree(path);
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
		if (explorer.path == SCENES_PATH) { explorer_confirm = "Save Scene"; explorer.option = Explorer::OPTION::SCENE; }
		else if (explorer.path == SCRIPTS_PATH) { explorer_confirm = "Export"; explorer.option = Explorer::OPTION::SCRIPT; }
		break;
	case Explorer::TYPE::LOAD:
		if (explorer.path == SCENES_PATH) { explorer_confirm = "Load Scene"; explorer.option = Explorer::OPTION::SCENE; }
		else if (explorer.path == SCRIPTS_PATH) { explorer_confirm = "Import"; explorer.option = Explorer::OPTION::SCRIPT; }
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
	DrawFiles(explorer.file_name);
	ImGui::Text("File Name:");
	ImGui::SameLine();
	if (explorer.type == Explorer::TYPE::LOAD) textinput_flags = ImGuiInputTextFlags_ReadOnly;
	ImGuiH::InputText("##Name", &explorer.file_name, textinput_flags);
	
	ImGui::SameLine();
	if (ImGuiH::ButtonAlignOnLine(explorer_confirm.c_str(), 1.0f))
	{
		CkeckDirectory(explorer.file_name);
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
				if (explorer.option == Explorer::OPTION::SCENE)
				{
					Serialization::YK_SaveScene(explorer.file_name);
				}
				else if (explorer.option == Explorer::OPTION::SCRIPT)
				{
					dynamic_cast<C_Blueprint*>(app->engine_order->editor->GetSelectedGameObject()->GetComponent(Component::TYPE::BLUEPRINT))->ExportBlueprint(explorer.file_name);
				}
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

void FileExplorer::DrawFiles(std::string& file_name)
{
	std::vector<std::string> file_paths = ModuleFile::FS_RemoveExtensions(directory->files);
	if (ImGui::ListBoxHeader("##Files in path", ImVec2(ImGui::GetWindowWidth(), 0)))
	{
		for (auto& file : file_paths)
		{
			if (explorer.option == Explorer::OPTION::SCENE)
			{
				if (file != app->GetSceneName() || explorer.type == Explorer::TYPE::SAVE)
				{
					std::string& item_name = file;
					if (ImGui::Selectable(item_name.c_str()))
					{
						file_name = file;
					}
				}
			}
			else if (explorer.option == Explorer::OPTION::SCRIPT)
			{
				std::string& item_name = file;
				if (ImGui::Selectable(item_name.c_str()))
				{
					file_name = file;
				}
			}
		}
		ImGui::ListBoxFooter();
	}
}

void FileExplorer::CkeckDirectory(std::string file_name)
{
	if (explorer.type == Explorer::TYPE::SAVE)
	{
		if (file_name != "")
		{
			if (explorer.option == Explorer::OPTION::SCENE)
			{
				if (std::find(directory->files.begin(), directory->files.end(), file_name + ".ykscene") == directory->files.end()) // file not found
				{
					Serialization::YK_SaveScene(file_name);
					app->SetSceneName(file_name);
					CloseExplorer();
				}
				else
				{
					ImGui::OpenPopup("Confirm Save");
					comfirm_popUp = true;
					ori = ImGui::GetMousePosOnOpeningCurrentPopup();
				}
			}
			else if (explorer.option == Explorer::OPTION::SCRIPT)
			{
				if (std::find(directory->files.begin(), directory->files.end(), file_name + ".ykbp") == directory->files.end()) // file not found
				{
					dynamic_cast<C_Blueprint*>(app->engine_order->editor->GetSelectedGameObject()->GetComponent(Component::TYPE::BLUEPRINT))->ExportBlueprint(file_name);
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
	}
	else if (explorer.type == Explorer::TYPE::LOAD)
	{
		if (file_name != "")
		{
			if (explorer.option == Explorer::OPTION::SCENE)
			{
				Serialization::YK_LoadScene(file_name);
				app->SetSceneName(file_name);
				CloseExplorer();
			}
			else if (explorer.option == Explorer::OPTION::SCRIPT)
			{
				dynamic_cast<C_Blueprint*>(app->engine_order->editor->GetSelectedGameObject()->GetComponent(Component::TYPE::BLUEPRINT))->LoadBlueprint(file_name);
				CloseExplorer();
			}
		}
	}
}