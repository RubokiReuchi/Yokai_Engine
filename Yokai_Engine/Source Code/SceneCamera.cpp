#include "SceneCamera.h"
#include "ModuleInput.h"
#include "GameObject.h"
#include "ModuleEngineOrder.h"
#include "EO_Editor.h"
#include "ModuleWindow.h"

SceneCamera::SceneCamera()
{
}

SceneCamera::~SceneCamera()
{
}

void SceneCamera::UpdateCameraInput(float dt)
{
	// Mouse motion ----------------
	if (editor == nullptr) editor = app->engine_order->editor;
	GameObject* selectedGO = editor->GetSelectedGameObject();

	float dx = -(float)app->input->GetMouseXMotion();
	float dy = -(float)app->input->GetMouseYMotion();
	float Sensitivity = 0.25f;

	float3 empty = { 0,0,0 };
	Quat lookingDir = Quat::identity;
	cameraFrustum.WorldMatrix().Decompose(empty, lookingDir, empty);

	float3 newPos(0, 0, 0);
	float speed = 5.0f * dt;
	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 50.0f * dt;

	// Mouse motion ----------------
	if (confine_move && app->input->GetKey(SDL_SCANCODE_LALT) == KEY_IDLE && app->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos += cameraFrustum.up * speed;
		if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) newPos -= cameraFrustum.up * speed;
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += cameraFrustum.front * speed;
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= cameraFrustum.front * speed;
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= cameraFrustum.WorldRight() * speed;
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += cameraFrustum.WorldRight() * speed;
		
		Position += newPos;
		Reference += newPos;

		cameraFrustum.pos += newPos;

		if (dy != 0)
		{
			float DeltaY = math::DegToRad((float)dy * Sensitivity * 0.75f);

			math::Quat rotation = Quat::identity;
			rotation.SetFromAxisAngle({ 1.0f, 0.0f, 0.0f }, DeltaY);

			lookingDir = lookingDir * rotation;
		}

		if (dx != 0)
		{
			float DeltaX = math::DegToRad((float)dx * Sensitivity * 0.75f);

			math::Quat rotation = Quat::identity;
			rotation.SetFromAxisAngle({ 0.0f, 1.0f, 0.0f }, DeltaX);

			lookingDir = rotation * lookingDir;
		}

		float4x4 newWorldMatrix = cameraFrustum.WorldMatrix();
		newWorldMatrix.SetRotatePart(lookingDir.Normalized());
		cameraFrustum.SetWorldMatrix(newWorldMatrix.Float3x4Part());
	}

	// pan
	if (confine_pan && app->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT)
	{
		if (dx != 0 || dy != 0)
		{
			float3 movVector((cameraFrustum.WorldRight() * dx) + (-cameraFrustum.up * dy));
			cameraFrustum.pos += movVector * dt;
		}
	}

	// rotate arround the selected game object
	if (confine_move && app->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && app->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		float3 rotationCenter = { 0.0f, 0.0f, 0.0f };

		if (selectedGO != nullptr) rotationCenter = selectedGO->transform->GetGlobalTransform().position;

		float distFromCenter = cameraFrustum.pos.Distance(rotationCenter);

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Quat rotation = Quat::identity;
			rotation.SetFromAxisAngle(float3(1, 0, 0), DeltaY * DEGTORAD);

			lookingDir = lookingDir * rotation;
		}

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			Quat rotation = Quat::identity;
			rotation.SetFromAxisAngle(float3(0, 1, 0), DeltaX * DEGTORAD);

			lookingDir = rotation * lookingDir;
		}

		float4x4 newWorldMatrix = cameraFrustum.WorldMatrix();
		newWorldMatrix.SetRotatePart(lookingDir.Normalized());
		cameraFrustum.SetWorldMatrix(newWorldMatrix.Float3x4Part());

		cameraFrustum.pos = rotationCenter + (cameraFrustum.front * -distFromCenter);
		LookAt(rotationCenter);
	}

	// zoom in and zoom out
	if (app->input->GetMouseZ() != 0)
	{
		float newFOV = math::RadToDeg(GetFOV()) + (1.5f * -app->input->GetMouseZ());

		if (newFOV > 20.0f && newFOV < 160.0f) SetFOV(newFOV);
	}

	// go to selected game object
	if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		if (selectedGO != nullptr)
		{
			Focus(selectedGO->transform->GetGlobalTransform().position);
		}
	}
}

void SceneCamera::Focus(const float3& focusPoint)
{
	float3 newPos = focusPoint;
	newPos += float3(4.0f, 0.0f, 2.0f);
	cameraFrustum.pos = newPos;
	LookAt(focusPoint);
}