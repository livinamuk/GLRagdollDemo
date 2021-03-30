#pragma once
#include "Header.h"
#include "Renderer/Model.h"
#include "bullet/src/btBulletCollisionCommon.h"
#include "Physics/Physics.h"

class Entity
{
public: // methods

	Entity();
	Entity(Model* model, GLuint textureID);
	void Update(float deltaTime);
	void DrawEntity(Shader* shader);
	void UpdateCollisionObject();
	void RemoveCollisionObject();
	void ConfigureDecalMap(int width, int height);

public:	// fields
	Transform m_transform;
	btCollisionObject* m_collisionObject;
	btTriangleMesh* m_triangleMesh;
	btBvhTriangleMeshShape* m_triangleMeshShape;
	GLuint m_decalMapFBO;
	GLuint m_decalMapID;
	GLuint m_albedoTextureID;

private: // field
	Model* m_model;
};