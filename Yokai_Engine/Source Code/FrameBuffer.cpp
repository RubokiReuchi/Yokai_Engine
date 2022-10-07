#include "FrameBuffer.h"

FrameBuffer::FrameBuffer()
{
}

FrameBuffer::~FrameBuffer()
{
	Clear();
}

void FrameBuffer::Clear()
{
	if (frameBuffer != 0)
		glDeleteFramebuffers(1, (GLuint*)&frameBuffer);

	if (texColorBuffer != 0)
		glDeleteTextures(1, (GLuint*)&texColorBuffer);

	if (rboDepthStencil != 0)
		glDeleteRenderbuffers(1, (GLuint*)&rboDepthStencil);
}

void FrameBuffer::CreateBuffer()
{
	//Clear();

	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	// generate texture
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

	// create render buffer object
	glGenRenderbuffers(1, &rboDepthStencil);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepthStencil);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1280, 720); //Size of the screen printed
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepthStencil);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		LOG("ERROR::FRAMEBUFFER:: Framebuffer is not complete!" );
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}