#pragma once

#include"MathGeoLib/include/Math/float2.h"
#include"OpenGL.h"
#include"Globals.h"

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	void Bind();

	void SetDimensions(int width, int height);
	void SetBufferInfo();

	uint GetTexture() { return texture_color_buffer; }

private:

	void RegenerateBuffer();
private:
	uint FBO = 0;
	uint texture_color_buffer = 0;
	uint RBO = 0;

	int width = SCREEN_WIDTH;
	int height = SCREEN_HEIGHT;
	friend class Camera;
};