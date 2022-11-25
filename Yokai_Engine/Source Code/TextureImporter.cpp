#include "TextureImporter.h"
#include "Application.h"
#include "ModuleFile.h"
#include "DevIL/include/il.h"
#include "DevIL/include/ilut.h"
#include "stb_image/stb_image.cpp"
#include "Re_Texture.h"
#include "M_Texture.h"

#pragma comment (lib, "DevIL/libx86/DevIL.lib")
#pragma comment (lib, "DevIL/libx86/ILU.lib")
#pragma comment (lib, "DevIL/libx86/ILUT.lib")

uint TextureImporter::LoadTexture(std::string path)
{
	std::string texture_path = TEXTURES_PATH + ModuleFile::FS_GetFileName(path, false) + ".dds";
	if (!ModuleFile::FS_Exists(texture_path))
	{
		// save custom format
		std::string file = TEXTURES_PATH;
		file += app->file->FS_GetFileName(path, false);
		file += ".dds";

		uint size = 0;
		char* buffer = YK_SaveTexture(size, path);

		app->file->FS_Save(file.c_str(), buffer, size, false);
		RELEASE_ARRAY(buffer);
	}

	return ImportTexture(texture_path);
}

char* TextureImporter::YK_SaveTexture(uint& size, std::string path)
{
	char* buffer = NULL;
	size = app->file->FS_Load(path, &buffer);

	ILuint image = 0;
	ilGenImages(1, &image);
	ilBindImage(image);
	if (!ilLoadL(IL_TYPE_UNKNOWN, buffer, size))
	{
		LOG("Image not loaded.");
	}

	ILuint n_size = 0;
	ILubyte* data = NULL;

	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
	n_size = ilSaveL(IL_DDS, nullptr, 0);
	if (n_size > 0)
	{
		data = new ILubyte[n_size];
		size = ilSaveL(IL_DDS, data, n_size);
	}

	ilDeleteImages(1, &image);

	return (char*)data;
}

uint TextureImporter::ImportTexture(std::string path)
{
	if (M_Texture::usedPaths.find(path) != M_Texture::usedPaths.end())
	{
		return M_Texture::usedPaths[path];
	}

	uint m_renderer_id;
	char* buffer = NULL;
	uint size = app->file->FS_Load(path, &buffer);

	ILuint image;
	ilGenImages(1, &image);
	ilBindImage(image);

	if (!ilLoadL(IL_TYPE_UNKNOWN, buffer, size))
	{
		LOG("Image not loaded.");
	}

	glGenTextures(1, &m_renderer_id);
	glBindTexture(GL_TEXTURE_2D, m_renderer_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
	ilDeleteImages(1, &image);
	glBindTexture(GL_TEXTURE_2D, 0);

	Re_Texture tex;
	tex.OpenGL_id = m_renderer_id;
	tex.name = path;

	M_Texture::loaded_textures[m_renderer_id] = tex; // Add loaded texture inside M_texture
	M_Texture::usedPaths[path] = m_renderer_id;

	return m_renderer_id;
}

uint TextureImporter::CreateTextureChecker()
{
	if (M_Texture::usedPaths.find("Checkers") != M_Texture::usedPaths.end())
	{
		return M_Texture::usedPaths["Checkers"];
	}

	GLubyte checkerImage[240][240][4];
	for (int i = 0; i < 240; i++) {
		for (int j = 0; j < 240; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}

	uint textureID = 0;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 240, 240, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);

	Re_Texture engineTexture;
	engineTexture.OpenGL_id = textureID;
	engineTexture.name = "Checkers";

	M_Texture::loaded_textures[textureID] = engineTexture; // Add loaded texture inside M_texture
	M_Texture::usedPaths["Checkers"] = textureID;

	return textureID;
}