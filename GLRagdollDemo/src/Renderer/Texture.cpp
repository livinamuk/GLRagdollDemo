#include "Texture.h"

std::vector<Texture> Texture::s_textures;

Texture::Texture(std::string name)
{
	this->name = name;
	ReadFromDisk();
	LoadToGL();
}

void Texture::LoadTexture(std::string filename)
{
	s_textures.push_back(Texture(filename));
}

GLuint Texture::GetTexIDByTexName(std::string name)
{
	for (Texture& texture : s_textures)
	{
		if (name == texture.name)
			return texture.ID;
	}
	return -1;
}

void Texture::ReadFromDisk()
{	
	stbi_set_flip_vertically_on_load(false);
	std::string fullpath = "res/textures/" + name;
	data = stbi_load(fullpath.c_str(), &width, &height, &nrChannels, 0);
}

void Texture::LoadToGL()
{
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	GLint format = GL_RGB;

	if (nrChannels == 4)
		format = GL_RGBA;
	if (nrChannels == 1)
		format = GL_RED;

	// Generate texture
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Lodoad texture: " << name << "\n";
	}
	else
		std::cout << "Failed to load texture: " << name << "\n";

	stbi_image_free(data);
}