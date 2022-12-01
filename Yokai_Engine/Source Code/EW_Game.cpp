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
	EO_Game* game = app->engine_order->game;

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

	if (ImGui::IsWindowHovered() && game->in_game && ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Left))
	{
		ClipCursor(&window_rect);
	}
	else if (ImGui::IsWindowHovered() && game->in_game && app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_UP)
	{
		ClipCursor(NULL);
	}

	ImGui::Image((ImTextureID)currentGameCamera->frameBuffer.GetTexture(), ImVec2(game_width, game_height), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::PopStyleVar();

	ImVec2 save_pos = ImGui::GetWindowPos();
	ImVec2 save_size = ImGui::GetWindowSize();
	ImGui::SetNextWindowPos(ImVec2(save_pos.x + (save_size.x / 2) - 54, save_pos.y + 25));
	ImGui::BeginChild("Time Control", ImVec2(108, 42), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

	std::string button1 = ICON_FA_PLAY"##1";
	if (app->engine_order->game->in_game) button1 = ICON_FA_STOP"##1";
	if (ImGui::Button((button1.c_str()), ImVec2(25, 25)))
	{
		if (!game->in_game) game->PlayGame();
		else game->StopGame();
	}
	ImGui::SameLine();
	std::string button2 = ICON_FA_PAUSE"##2";
	if (app->engine_order->game->paused) button2 = ICON_FA_PLAY"##2";
	if (ImGui::Button((button2.c_str()), ImVec2(25, 25)))
	{
		if (game->in_game)
		{
			if (!game->paused) game->PauseGame();
			else game->ContinueGame();
		}
	}
	ImGui::SameLine();
	if (ImGui::Button((ICON_FA_FORWARD"##3"), ImVec2(25, 25)))
	{
		if (game->paused) game->tick = true;
	}

	ImGui::EndChild();
	ImGui::End();
}
