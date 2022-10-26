#include "EW_Scene.h"
#include "Application.h"
#include "EO_Editor.h"
#include "ModuleInput.h"
#include "ModuleFile.h"
#include "MeshImporter.h"
#include "TextureImporter.h"

#include <math.h>

EW_Scene::EW_Scene()
{
	window_name = "Scene";
	enabled = true;

	scene_camera = &app->camera->sceneCamera;
}

EW_Scene::~EW_Scene()
{
}

void EW_Scene::Update()
{
	// Screen
	ImGui::Begin(window_name.c_str(), &enabled, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	
	scene_camera->active = true;
	app->camera->updateSceneCamera = (bool)ImGui::IsWindowHovered();
	ImVec2 gameDimensions = ImGui::GetContentRegionAvail();

	if (gameDimensions.x != scene_width || gameDimensions.y != scene_height)
	{
		// If the size of this imgui window is different from the one stored.
		scene_width = (int)gameDimensions.x;
		scene_height = (int)gameDimensions.y;

		app->camera->sceneCamera.ChangeAspectRatio((float)scene_width / (float)scene_height);
	}

	ImGui::Image((ImTextureID)scene_camera->frameBuffer.GetTexture(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));

	if (ImGui::IsWindowHovered() && app->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_DOWN) scene_camera->confine_move = true;
	else if (ImGui::IsWindowHovered() && app->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_UP) scene_camera->confine_move = false;
	if (ImGui::IsWindowHovered() && app->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_DOWN) scene_camera->confine_pan = true;
	else if (ImGui::IsWindowHovered() && app->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_UP) scene_camera->confine_pan = false;

	if ((scene_camera->confine_move && app->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		|| scene_camera->confine_pan && app->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT)
	{
		uint dx = math::Abs(app->input->GetMouseXMotion());
		uint dy = math::Abs(app->input->GetMouseYMotion());
		if (ImGui::GetMousePos().x - 2 - dx < ImGui::GetWindowPos().x)
		{
			app->input->SetMousePos(ImGui::GetWindowPos().x + ImGui::GetWindowWidth() - 5, ImGui::GetMousePos().y);
		}
		else if (ImGui::GetMousePos().x + 4 + dx > ImGui::GetWindowPos().x + ImGui::GetWindowWidth())
		{
			app->input->SetMousePos(ImGui::GetWindowPos().x + 3, ImGui::GetMousePos().y);
		}
		else if (ImGui::GetMousePos().y - 22 - dy < ImGui::GetWindowPos().y)
		{
			app->input->SetMousePos(ImGui::GetMousePos().x, ImGui::GetWindowPos().y + ImGui::GetWindowHeight() - 12);
		}
		else if (ImGui::GetMousePos().y + 11 + dy > ImGui::GetWindowPos().y + ImGui::GetWindowHeight())
		{
			app->input->SetMousePos(ImGui::GetMousePos().x, ImGui::GetWindowPos().y + 23);
		}
	}

	if (ImGui::BeginDragDropTarget() && app->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
	{
		std::string dd_file = app->engine_order->editor->dd_file;
		app->engine_order->editor->dd_file = "";
		ImGuiDragDropFlags target_flags = 0;
		target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;
		target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(dd_file.c_str(), target_flags))
		{
			switch (app->file->S_GetResourceType(dd_file))
			{
			case RE_TYPE::MESH:
				MeshImporter::LoadMesh(dd_file);
				break;
			case RE_TYPE::TEXTURE:
				TextureImporter::ImportTextureSTBI(dd_file);
				break;
			case RE_TYPE::UNDEFINED:
				break;
			}
		}
		ImGui::EndDragDropTarget();
	}
	ImGui::End();
}