#include "M_Texture.h"
#include "Application.h"
#include "ModuleEngineOrder.h"
#include "DevIL/include/il.h"
#include "DevIL/include/ilut.h"
#include "TextureImporter.h"

std::map<uint, Re_Texture> M_Texture::loaded_textures;
std::map<std::string, uint> M_Texture::usedPaths;
std::map<std::string, uint> M_Texture::texture_image;
std::vector<M_Texture::BindedTextureInfo> M_Texture::bindedTexturesInfo;
uint M_Texture::bindedTextures = 0;

float M_Texture::BindTexture(uint texture)
{
	for (size_t i = 0; i < bindedTexturesInfo.size(); i++)
	{
		if (bindedTexturesInfo[i].Texture_id == texture)
		{
			return (float)bindedTexturesInfo[i].OpenGL_slot_id;
		}
	}

	if (GL_TEXTURE0 + bindedTextures == GL_TEXTURE31)
		return -1;

	glActiveTexture(GL_TEXTURE0 + bindedTextures);
	glBindTexture(GL_TEXTURE_2D, texture);

	bindedTexturesInfo.push_back({ texture, bindedTextures });

	return (float)bindedTextures++;
}

void M_Texture::UnBindTextures()
{
	bindedTextures = 0;
	bindedTexturesInfo.clear();

	for (int i = 0; i < 32; i++)
	{
		glBindTexture(GL_TEXTURE0 + i, 0); // Unbind every texture
	}
}

void M_Texture::DeleteTextures()
{
	texture_image.clear();
	for (auto& texture : loaded_textures)
	{
		glDeleteTextures(1, &texture.first);
	}
	loaded_textures.clear();
	usedPaths.clear();
}