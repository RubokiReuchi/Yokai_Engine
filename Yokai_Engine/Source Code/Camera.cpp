#include "Camera.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"

Camera::Camera()
{
	CalculateViewMatrix();

	Position = float3(0.0f, 0.0f, 0.0f);
	X = float3(1.0f, 0.0f, 0.0f);
	Y = float3(0.0f, 1.0f, 0.0f);
	Z = float3(0.0f, 0.0f, 1.0f);

	cameraFrustum.nearPlaneDistance = 0.1f;
	cameraFrustum.farPlaneDistance = range;
	cameraFrustum.type = math::FrustumType::PerspectiveFrustum;
	cameraFrustum.verticalFov = FOV = math::DegToRad(60.0f);
	aspectRatio = (16.0f / 9.0f);
	cameraFrustum.horizontalFov = 2.0f * atanf(tanf(cameraFrustum.verticalFov / 2.0f) * aspectRatio);	
	cameraFrustum.pos = Position;
	cameraFrustum.front = Z;
	cameraFrustum.up = Y;
}

Camera::~Camera()
{
}

void Camera::LookAt(const float3& objective)
{
	cameraFrustum.front = (objective - cameraFrustum.pos).Normalized();
	float3 aux = float3(0, 1, 0).Cross(cameraFrustum.front).Normalized();
	cameraFrustum.up = cameraFrustum.front.Cross(aux);
}

void Camera::Move(const float3& movement)
{
	cameraFrustum.pos += movement;
}

float* Camera::GetViewMatrix()
{
	ViewMatrix = cameraFrustum.ViewMatrix();
	ViewMatrix.Transpose();
	return &ViewMatrix.v[0][0];
}

float* Camera::GetProjectionMatrix()
{
	ProjectionMatrix = cameraFrustum.ProjectionMatrix().Transposed();
	return &ProjectionMatrix.v[0][0];
}

void Camera::RecalculateProjection()
{
	float aspectRatio = (float)(frameBuffer.width) / (float)(frameBuffer.height);
	cameraFrustum.horizontalFov = 2.f * atanf(tanf(cameraFrustum.verticalFov * 0.5f) * aspectRatio);
}

void Camera::RegenerateFrameBuffer(int width, int height)
{
	frameBuffer.SetDimensions(width, height);
	RecalculateProjection();
}

void Camera::ChangeAspectRatio(float aspectRatio)
{
	this->aspectRatio = aspectRatio;
	cameraFrustum.horizontalFov = 2.f * atanf(tanf(cameraFrustum.verticalFov * 0.5f) * aspectRatio);
}

void Camera::SetFOV(float fov)
{
	cameraFrustum.verticalFov = FOV = math::DegToRad(fov);
	cameraFrustum.horizontalFov = 2.0f * atanf(tanf(cameraFrustum.verticalFov / 2.0f) * aspectRatio);
}

void Camera::SetRange(float range)
{
	this->range = range;
	cameraFrustum.farPlaneDistance = range;
}

void Camera::CheckGoInCamera()
{
	for (auto& go : app->engine_order->game_objects)
	{
		if (!go.second->GetComponent(Component::TYPE::MESH_RENDERER))
		{
			continue;
		}

		bool ret = true;
		float3 corner[8];
		int total = 0;
		go.second->global_aabb.GetCornerPoints(corner);

		for (size_t p = 0; p < 6; p++)
		{
			int count = 8;
			int pt = 1;

			for (size_t i = 0; i < 8; i++)
			{
				if (cameraFrustum.GetPlane(p).IsOnPositiveSide(corner[i]))
				{
					pt = 0;
					count--;
				}
			}

			if (count == 0)
			{
				ret = false;
			}

			total += pt;
		}

		dynamic_cast<C_MeshRenderer*>(go.second->GetComponent(Component::TYPE::MESH_RENDERER))->GetMesh().in_camera = ret;
	}
}

void Camera::CalculateViewMatrix()
{
	
}