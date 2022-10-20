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

	float3 newPos(0, 0, 0);
	float speed = 5.0f * dt;
	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 50.0f * dt;

	// Mouse motion ----------------

	if (app->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos += cameraFrustum.up * speed;
		if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) newPos -= cameraFrustum.up * speed;
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= cameraFrustum.front * speed;
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += cameraFrustum.front * speed;
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos += cameraFrustum.WorldRight() * speed;
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos -= cameraFrustum.WorldRight() * speed;
		
		Position += newPos;
		Reference += newPos;

		int dx = -app->input->GetMouseXMotion();
		int dy = -app->input->GetMouseYMotion();

		float Sensitivity = 0.25f;

		Position -= Reference;

		if (dx != 0)
		{
			float DeltaX = math::DegToRad((float)dx * Sensitivity);

			math::Quat rotation;
			rotation.SetFromAxisAngle({ 0.0f, 1.0f, 0.0f }, DeltaX);

			X = rotation * X;
			Y = rotation * Y;
			Z = rotation * Z;
		}

		if (dy != 0)
		{
			float DeltaY = math::DegToRad((float)dy * Sensitivity);

			math::Quat rotation;
			rotation.SetFromAxisAngle(X, DeltaY);

			Y = rotation * Y;
			Z = rotation * Z;

			if (Y.y < 0.0f)
			{
				Z = float3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = Cross(Z, X);
			}
		}

		Position = Reference + Z * Position.Length();
	}

	// Recalculate matrix -------------
	CalculateViewMatrix();
}

void SceneCamera::Focus(const float3& focusPoint)
{
	float3 newPos = focusPoint;
	newPos += float3(4.0f, 0.0f, 2.0f);
	cameraFrustum.pos = newPos;
	LookAt(focusPoint);
}