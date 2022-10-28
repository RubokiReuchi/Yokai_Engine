#include "TextureImporter.h"
#include "DevIL/include/il.h"
#include "DevIL/include/ilut.h"
#include "stb_image/stb_image.cpp"
#include "Re_Texture.h"
#include "M_Texture.h"

#pragma comment (lib, "DevIL/libx86/DevIL.lib")
#pragma comment (lib, "DevIL/libx86/ILU.lib")
#pragma comment (lib, "DevIL/libx86/ILUT.lib")

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

uint TextureImporter::ImportTexture(std::string path)
{
	if (M_Texture::usedPaths.find(path) != M_Texture::usedPaths.end())
	{
		return M_Texture::usedPaths[path];
	}

	uint m_renderer_id;
	std::string m_file_path;
	unsigned char* m_local_buffer;
	int m_Width, m_Height, m_BPP; // BPP = Bits per pixel

	m_local_buffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	glGenTextures(1, &m_renderer_id);
	glBindTexture(GL_TEXTURE_2D, m_renderer_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_local_buffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (m_local_buffer)
	{
		stbi_image_free(m_local_buffer);
	}

	Re_Texture engineTexture;
	engineTexture.OpenGL_id = m_renderer_id;
	engineTexture.name = path;

	M_Texture::loaded_textures[m_renderer_id] = engineTexture; // Add loaded texture inside M_texture
	M_Texture::usedPaths[path] = m_renderer_id;

	return m_renderer_id;
}