#pragma once
#include "Core/Input.h"
#include "Renderer/Transform.h"

class Camera
{
public:	// Methods
	Camera();
	void CalculateMatrices();
	void CalculateProjectionMatrix(int screenWidth, int screenHeight);
	void CalculateWeaponSwayTransform(float deltatime);
	void Update(float deltaTime);
	void MouseLook(float deltaTime);

private: // Methods
	void PlayerMovement(float deltaTime);

public: // Fields
	float m_oldX, m_oldY;		// Old mouse position
	float m_xoffset, m_yoffset;	// Distance mouse moved during current frame in pixels
	bool m_disable_MouseLook = false;

	Transform m_transform;

	glm::vec3 m_Front = glm::vec3(0, 1, 0);
	glm::vec3 m_Up;
	glm::vec3 m_Right;
	glm::vec3 m_WorldUp;

	glm::mat4 m_projectionViewMatrix;
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;
	glm::mat4 m_inverseViewMatrix;
	glm::mat4 m_inversePprojectionMatrix;

	float m_mmouseSensitivity;
		
	glm::vec3 m_viewPos = glm::vec3(0);
	float m_viewHeight = 1.0f;

	int m_disableMouseLookTimer = 0;

};
