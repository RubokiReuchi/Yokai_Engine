#pragma once

#include"Resource.h"
#include<string>
#include"Color.h"
#include"Rect.h"

typedef unsigned int GLuint;


class ResourceTexture : public Resource
{
public:
	ResourceTexture(unsigned int _uid);
	~ResourceTexture();

	bool LoadToMemory() override;
	bool UnloadFromMemory() override;
	Rect GetTextureChunk(Rect area);

public:
	int tWidth;
	int tHeight;

	GLuint textureID;
	Color color;

};