#pragma once
#include "Header.h"

#include <stb_image.h>

class Texture
{
public: // Methods
	Texture(std::string filename);
	static void LoadTexture(std::string filename);
	static GLuint GetTexIDByTexName(std::string name);

private: 
	void ReadFromDisk();
	void LoadToGL();

public: // fields
	unsigned int ID = 0;
	std::string name;
	int width, height;

private:
	unsigned char* data;
	int nrChannels;
	
	static std::vector<Texture> s_textures;
};

