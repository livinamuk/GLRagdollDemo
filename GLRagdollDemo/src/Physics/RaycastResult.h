#pragma once

#include "Physics.h"
#include "Core/Camera.h"
#include "Helpers/Util.h"

class RaycastResult 
{
public: // methods
	RaycastResult();
	void CastRay(glm::vec3 rayOrigin, glm::vec3 rayDirection, float rayLength);
	static RaycastResult CastMouseRay(Camera* camera);

public: // members
	//int m_elementIndex; 
	float m_distance;
	glm::vec3 m_hitPoint;
	glm::vec3 m_surfaceNormal;
	void* m_objectPtr;
	btRigidBody* m_rigidBody = nullptr;
	PhysicsObjectType m_objectType;// char* m_name;
};