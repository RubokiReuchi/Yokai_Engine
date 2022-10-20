#include "EW_Game.h"
#include "ModuleCamera3D.h"
#include "Camera.h"

EW_Game::EW_Game()
{
	window_name = "Game";
	currentGameCamera = app->camera->activeGameCamera;
}

EW_Game::~EW_Game()
{
}

void EW_Game::Update()
{
	// Screen
	ImGui::Begin(window_name.c_str(), &enabled, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	currentGameCamera->active = true;
	app->camera->updateGameCamera = (bool)ImGui::IsWindowHovered();
	ImVec2 gameDimensions = ImGui::GetContentRegionAvail();

	if (gameDimensions.x != game_width || gameDimensions.y != game_height)
	{
		// If the size of this imgui window is different from the one stored.
		game_width = (int)gameDimensions.x;
		game_height = (int)gameDimensions.y;

		app->camera->sceneCamera.ChangeAspectRatio((float)game_width / (float)game_height);
	}

	ImGui::Image((ImTextureID)currentGameCamera->frameBuffer.GetTexture(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
}
