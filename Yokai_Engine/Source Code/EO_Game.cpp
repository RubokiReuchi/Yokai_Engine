#include "EO_Game.h"
#include "ModuleEngineOrder.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "EO_Editor.h"

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
		if (!paused && app->engine_order->editor->GetGameWindow()->mouse_confined)
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
	SaveGameObject();
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
	ReturnGameObject();
}

void EO_Game::SaveGameObject()
{
	for (auto& go : app->engine_order->game_objects)
	{
		GameObjectInfo aux;
		aux.parent = go.second->parent;
		aux.name = go.second->name;
		aux.enabled = go.second->enabled;
		aux.visible = go.second->visible;
		aux.transform = go.second->transform->GetLocalTransform();
		C_Material* mat = dynamic_cast<C_Material*>(go.second->GetComponent(Component::TYPE::MATERIAL));
		if (mat != NULL) aux.texture_path = mat->GetTexture();

		go_before_play[go.second->id] = aux;
	}
}

void EO_Game::ReturnGameObject()
{
	for (size_t i = 2; i < go_before_play.size() + 1; i++)
	{
		if (go_before_play[i].parent->id != app->engine_order->game_objects[i]->parent->id) app->engine_order->game_objects[i]->SetParent(go_before_play[i].parent);
		app->engine_order->game_objects[i]->name = go_before_play[i].name;
		app->engine_order->game_objects[i]->enabled = go_before_play[i].enabled;
		app->engine_order->game_objects[i]->visible = go_before_play[i].visible;

		if (go_before_play[i].transform.position.x != app->engine_order->game_objects[i]->transform->GetLocalTransform().position.x
			|| go_before_play[i].transform.position.y != app->engine_order->game_objects[i]->transform->GetLocalTransform().position.y
			|| go_before_play[i].transform.position.z != app->engine_order->game_objects[i]->transform->GetLocalTransform().position.z
			|| go_before_play[i].transform.rotation.x != app->engine_order->game_objects[i]->transform->GetLocalTransform().rotation.x
			|| go_before_play[i].transform.rotation.y != app->engine_order->game_objects[i]->transform->GetLocalTransform().rotation.y
			|| go_before_play[i].transform.rotation.z != app->engine_order->game_objects[i]->transform->GetLocalTransform().rotation.z
			|| go_before_play[i].transform.scale.x != app->engine_order->game_objects[i]->transform->GetLocalTransform().scale.x
			|| go_before_play[i].transform.scale.y != app->engine_order->game_objects[i]->transform->GetLocalTransform().scale.y
			|| go_before_play[i].transform.scale.z != app->engine_order->game_objects[i]->transform->GetLocalTransform().scale.z)
		{
			app->engine_order->game_objects[i]->transform->SetTransform(go_before_play[i].transform.position, go_before_play[i].transform.scale, go_before_play[i].transform.rotation);
		}

		if (go_before_play[i].texture_path != "donthavetexture") // default value
		{
			dynamic_cast<C_Material*>(app->engine_order->game_objects[i]->GetComponent(Component::TYPE::MATERIAL))->SetTexture(go_before_play[i].texture_path);
		}

		C_Blueprint* c_bp = dynamic_cast<C_Blueprint*>(app->engine_order->game_objects[i]->GetComponent(Component::TYPE::BLUEPRINT));
		if (c_bp != NULL)
		{
			c_bp->GetBluePrint()->info_saved_in_nodes = false;
		}
	}

	go_before_play.clear();

	for (auto& go : app->engine_order->game_objects)
	{
		if (go.second->temp_type != TempGoType::NONE)
		{
			go.second->DeleteGameObject();
		}
	}
}