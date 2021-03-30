#pragma once
#include <string>
#include <vector>


#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>


#include "Renderer/Mesh.h"
#include "Header.h"

class Model
{
public:
	Model();
	Model(const char* filepath);
	~Model();

	void ReadFromDisk();
	void LoadMeshDataToGL();
	void Draw(Shader* shader, glm::mat4 modelMatrix);
	void DrawMesh(Shader* shader, int meshIndex, glm::mat4 modelMatrix);
	//void Set_Mesh_MaterialID_Set0(const char* meshname, unsigned int materialID);
	//void Set_Mesh_MaterialID_Set1(const char* meshname, unsigned int materialID);
	
public:
	std::vector<Mesh*> m_meshes;

public:
	std::string name;
	std::string m_filePath;
	bool m_readFromDisk = false;
	bool m_loadedToGL = false;
	FileType m_fileType;
};
