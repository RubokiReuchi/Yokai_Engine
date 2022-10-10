#include "TextureImporter.h"
#include "DevIL/include/il.h"
#include "DevIL/include/ilut.h"
#include "Re_Texture.h"
#include "M_Texture.h"

uint TextureImporter::ImportTexture(std::string path)
{
	//Check if the given texture has been already loaded
	if (M_Texture::usedPaths.find(path) != M_Texture::usedPaths.end())
	{
		return M_Texture::usedPaths[path]; // If this texture path was already loaded, return the loaded texture.
	}

	ILuint ImgId = 0;
	ilGenImages(1, &ImgId);
	ilBindImage(ImgId);
	ilLoadImage(path.c_str());

	GLuint texture = ilutGLBindTexImage();

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	ilDeleteImages(1, &ImgId);

	Re_Texture engineTexture;
	engineTexture.OpenGL_id = texture;
	engineTexture.name = path;

	M_Texture::loadedTextures[texture] = engineTexture; // Add loaded texture inside TextureManager.
	M_Texture::usedPaths[path] = texture;

	return texture;
}