#include "EO_Game.h"
#include "ModuleEngineOrder.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

#include "ModuleFile.h"

EO_Game::EO_Game()
{
	timer = app->game_timer;
}

EO_Game::~EO_Game()
{

}

void EO_Game::Start()
{
	
}

void EO_Game::PreUpdate()
{

}

void EO_Game::Update()
{
	// game logic
	if (in_game)
	{
		if (!paused)
		{
			timer->frameCount++;
			timer->time += timer->realTime_dt;
			timer->dt = timer->realTime_dt * timer->time_scale;
			for (auto& go : app->engine_order->game_objects)
			{
				go.second->UpdateInGame(timer->dt);
			}
		}

		if (tick)
		{
			timer->frameCount++;
			timer->time += timer->realTime_dt;
			timer->dt = timer->realTime_dt * timer->time_scale;
			for (auto& go : app->engine_order->game_objects)
			{
				go.second->UpdateInGame(timer->dt);
			}
			tick = false;
		}
	}
}

void EO_Game::PostUpdate()
{
	
}

void EO_Game::CleanUp()
{
	
}

void EO_Game::PlayGame()
{
	in_game = true;
	timer->time_scale = 1.0f;
}

void EO_Game::PauseGame()
{
	paused = true;
}

void EO_Game::ContinueGame()
{
	paused = false;
}

void EO_Game::StopGame()
{
	in_game = false;
	paused = false;
	timer->Reset();
}