#pragma once

#include "FrameBuffer.h"
#include "glmath.h"
#include "MathGeoLib/include/MathGeoLib.h"

class Camera
{
public:
	Camera();
	~Camera();

	void LookAt(const float3& Spot);
	void Move(const float3& Movement);
	float* GetViewMatrix();
	float* GetProjectionMatrix();

	void RecalculateProjection();

	void RegenerateFrameBuffer(int width, int height);

	void ChangeAspectRatio(float aspectRatio);
	float GetAspectRatio() { return aspectRatio; }

	void SetFOV(float fov);
	float GetFOV() { return FOV; };

	void SetRange(float range);
	float GetRange() { return range; };

	void CheckGoInCamera();

public:
	float3 X, Y, Z, Position, Reference;
	bool active = true;
	FrameBuffer frameBuffer;
protected:
	void CalculateViewMatrix();

protected:
	float4x4 ViewMatrix, ViewMatrixInverse, ProjectionMatrix;
	bool changeFOVWithBufferSize;
	float aspectRatio = 0.0f;
	float FOV = 60.0f;
	float range = 1000.0f;

public:
	Frustum cameraFrustum;

	friend class ModuleCamera3D;
	friend class C_Camera;
};