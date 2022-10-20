#pragma once

#include "MathGeoLib/include/MathGeoLib.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

class GameObject;

class Component
{
public:

	enum class TYPE
	{
		NONE,
		TRANSFORM,
		MESH_RENDERER,
		MATERIAL,
		CAMERA,
	};

	Component(GameObject* game_object, TYPE type);
	virtual ~Component();

	bool IsEnabled() { return enabled; }
	GameObject* GetGameObject() { return go; }
	TYPE GetType() { return type; }

	virtual void OnEditor() {}

	// c_transform
	virtual void OnPositionUpdate(float3 position) {}
	virtual void OnScaleUpdate(float3 scale) {}
	virtual void OnRotationUpdate(float3 rotation) {}
	virtual void OnTransformUpdate(float3 position, float3 scale, float3 rotation) {}

private:
	bool enabled = true;
	GameObject* go = nullptr;
	TYPE type = TYPE::NONE;

};