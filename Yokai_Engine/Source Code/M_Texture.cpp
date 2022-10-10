#include "M_Texture.h"
#include "DevIL/include/il.h"
#include "DevIL/include/ilut.h"
#include "TextureImporter.h"

std::map<uint, Re_Texture> TextureManager::loadedTextures;
std::map<std::string, uint> TextureManager::usedPaths;
uint TextureManager::bindedTextures = 0;

float TextureManager::BindTexture(uint texture)
{
	if (GL_TEXTURE0 + bindedTextures == GL_TEXTURE31) // If we loaded 32 textures already
		return -1;

	glActiveTexture(GL_TEXTURE0 + bindedTextures);
	glBindTexture(GL_TEXTURE0 + bindedTextures, texture);

	return bindedTextures++;
}

void TextureManager::UnBindTextures()
{
	bindedTextures = 0;

	for (int i = 0; i < 32; i++)
	{
		glBindTexture(GL_TEXTURE0 + i, 0); // Unbind every texture
	}
}