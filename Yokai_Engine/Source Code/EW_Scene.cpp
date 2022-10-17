#include "EW_Scene.h"

EW_Scene::EW_Scene()
{
	window_name = "Scene";
	enabled = true;
}

EW_Scene::~EW_Scene()
{
}

void EW_Scene::Update()
{
	// Screen
	ImGui::Begin(window_name.c_str(), &enabled, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	ImVec2 gameDimensions = ImGui::GetContentRegionAvail();

	if (gameDimensions.x != game_width || gameDimensions.y != game_height)
	{
		// If the size of this imgui window is different from the one stored.
		game_width = (int)gameDimensions.x;
		game_height = (int)gameDimensions.y;

		app->renderer3D->frameBuffer.SetDimensions(game_width, game_height);
	}

	ImGui::Image((ImTextureID)app->renderer3D->frameBuffer.GetTexture(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
}