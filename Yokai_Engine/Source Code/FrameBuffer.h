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

	inline unsigned int GetFrameBuffer() { return frameBuffer; }
	inline unsigned int GetTextureBuffer() { return texColorBuffer; }
	inline unsigned int GetRBO() { return rboDepthStencil; }

private:

	uint frameBuffer;
	uint texColorBuffer;
	uint rboDepthStencil;
};