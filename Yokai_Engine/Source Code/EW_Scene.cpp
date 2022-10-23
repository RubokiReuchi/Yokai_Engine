#include "EW_Scene.h"
#include "Application.h"
#include "ModuleInput.h"

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

	if (ImGui::IsWindowHovered() && app->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		if (ImGui::GetMousePos().x - 2 < ImGui::GetWindowPos().x)
		{
			app->input->SetMousePos(ImGui::GetWindowPos().x + ImGui::GetWindowWidth() - 5, ImGui::GetMousePos().y);
		}
		else if (ImGui::GetMousePos().x + 4 > ImGui::GetWindowPos().x + ImGui::GetWindowWidth())
		{
			app->input->SetMousePos(ImGui::GetWindowPos().x + 3, ImGui::GetMousePos().y);
		}
		else if (ImGui::GetMousePos().y - 22 < ImGui::GetWindowPos().y)
		{
			app->input->SetMousePos(ImGui::GetMousePos().x, ImGui::GetWindowPos().y + ImGui::GetWindowHeight() - 12);
		}
		else if (ImGui::GetMousePos().y + 11 > ImGui::GetWindowPos().y + ImGui::GetWindowHeight())
		{
			app->input->SetMousePos(ImGui::GetMousePos().x, ImGui::GetWindowPos().y + 23);
		}
	}
	ImGui::End();
}