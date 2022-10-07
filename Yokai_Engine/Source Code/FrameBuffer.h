#pragma once

#include"MathGeoLib/include/Math/float2.h"
#include"OpenGL.h"
#include"Globals.h"

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	void Clear();

	void CreateBuffer();

	uint GetFrameBuffer() { return frameBuffer; }
	uint GetTextureBuffer() { return texColorBuffer; }
	uint GetRBO() { return rboDepthStencil; }

private:

	uint frameBuffer;
	uint texColorBuffer;
	uint rboDepthStencil;
};