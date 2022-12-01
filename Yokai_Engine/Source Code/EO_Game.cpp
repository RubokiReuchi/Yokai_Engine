#include "EO_Game.h"
#include "ModuleEngineOrder.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

#include "ModuleFile.h"

EO_Game::EO_Game()
{
}

EO_Game::~EO_Game()
{
}

void EO_Game::Start()
{
	
}

void EO_Game::PreUpdate()
{}

void EO_Game::Update()
{
	// game logic
	if (in_game && !paused)
	{
		for (auto& go : app->engine_order->game_objects)
		{
			go.second->UpdateInGame();
		}
	}

	if (tick)
	{
		for (auto& go : app->engine_order->game_objects)
		{
			go.second->UpdateInGame();
		}
		tick = false;
	}
}


void EO_Game::PostUpdate()
{
	
}

void EO_Game::CleanUp()
{
	
}