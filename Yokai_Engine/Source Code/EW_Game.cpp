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
	ImGui::Begin(window_name.c_str(), &enabled, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);

	currentGameCamera->active = true;
	app->camera->updateGameCamera = (bool)ImGui::IsWindowHovered();
	ImVec2 gameDimensions = ImGui::GetContentRegionAvail();

	if (gameDimensions.x != game_width)
	{
		// If the size of this imgui window is different from the one stored.
		game_width = gameDimensions.x;
		game_height = game_width * (9.0f / 16.0f);

		currentGameCamera->ChangeAspectRatio(game_width / game_height);
	}

	ImGui::Image((ImTextureID)currentGameCamera->frameBuffer.GetTexture(), ImVec2(game_width, game_height), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
}
