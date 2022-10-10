#include "TextureImporter.h"
#include "DevIL/include/il.h"
#include "DevIL/include/ilut.h"
#include "stb_image/stb_image.h"
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
	if (!ilLoadImage(path.c_str()))
	{
		LOG("Error loading image: %s", ilutGetString(ilGetError()));
	}

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

	M_Texture::loaded_textures[texture] = engineTexture; // Add loaded texture inside TextureManager.
	M_Texture::usedPaths[path] = texture;

	return texture;
}

uint TextureImporter::CheckerImage()
{
	GLubyte checker_image[240][240][4];
	for (int i = 0; i < 240; i++) {
		for (int j = 0; j < 240; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checker_image[i][j][0] = (GLubyte)c;
			checker_image[i][j][1] = (GLubyte)c;
			checker_image[i][j][2] = (GLubyte)c;
			checker_image[i][j][3] = (GLubyte)255;
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 240, 240,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checker_image);

	return textureID;
}

uint TextureImporter::ImportTextureSTBI(std::string path)
{
	uint m_renderer_id;
	std::string m_file_path;
	unsigned char* m_local_buffer;
	int m_Width, m_Height, m_BPP;	// BPP = Bits per pixel

	//stbi_set_flip_vertically_on_load(1);
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
	return m_renderer_id;
}