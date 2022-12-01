#include "EW_Game.h"
#include "EO_Game.h"
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
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin(window_name.c_str(), &enabled, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);
	window_rect.top = (LONG)(ImGui::GetWindowPos().y + ImGui::GetFrameHeight() + 4);
	window_rect.bottom = (LONG)(ImGui::GetWindowPos().y + ImGui::GetWindowHeight() - 4);
	window_rect.left = (LONG)(ImGui::GetWindowPos().x + 4);
	window_rect.right = (LONG)(ImGui::GetWindowPos().x + ImGui::GetWindowWidth() - 4);
	
	currentGameCamera->active = true;
	app->camera->updateGameCamera = (bool)ImGui::IsWindowHovered();
	ImVec2 gameDimensions = ImGui::GetWindowSize();

	if (gameDimensions.x != game_width)
	{
		game_width = gameDimensions.x;
		game_height = game_width * (9.0f / 16.0f);
		ImGui::SetWindowSize(ImVec2(ImGui::GetWindowSize().x, ImGui::GetWindowDpiScale()));
	}
	if (gameDimensions.y < game_height)
	{
		game_height = gameDimensions.y;
		game_width = game_height * (16.0f / 9.0f);
	}

	if (ImGui::IsWindowHovered() && !app->engine_order->game->in_game && ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Left))
	{
		app->engine_order->game->in_game = true;
		ClipCursor(&window_rect);
	}
	else if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_UP)
	{
		app->engine_order->game->in_game = false;
		ClipCursor(NULL);
	}

	ImGui::Image((ImTextureID)currentGameCamera->frameBuffer.GetTexture(), ImVec2(game_width, game_height), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::PopStyleVar();
	ImGui::End();

	//ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowPos().x + (ImGui::GetWindowWidth() / 2), ImGui::GetWindowPos().y + 25));
	ImGui::Begin("Time Control", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize /*| ImGuiWindowFlags_NoMove*/ | ImGuiWindowFlags_NoDocking);
	


	ImGui::End();
}
