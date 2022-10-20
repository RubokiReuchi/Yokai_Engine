#include "EW_Scene.h"

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
	ImGui::End();
}