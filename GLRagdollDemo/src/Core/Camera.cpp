#pragma once
//#include "hellpch.h"
#include "Camera.h"
#include "Header.h"
#include "Helpers/Util.h"
#include "Core/CoreGL.h"
//#include "Core/CoreImGui.h"
//#include "Config.h"

Camera::Camera()
{
	m_viewHeight = 1.3f;
	m_transform.position = glm::vec3(0, m_viewHeight, 2);
	m_transform.rotation = glm::vec3(-HELL_PI, -3.14, 0);
	m_transform.rotation = glm::vec3(0, 0, 0);
	m_transform.scale = glm::vec3(1);
	m_oldX = 1280 / 2;
	m_oldY = 720 / 2;
	Input::s_mouseX = (int)m_oldX;
	Input::s_mouseY = (int)m_oldY;
}

void Camera::CalculateMatrices()
{
	m_viewMatrix = glm::inverse(m_transform.to_mat4());
	m_inverseViewMatrix = glm::inverse(m_viewMatrix);

	glm::mat4 worldMatrix = m_transform.to_mat4();

	m_Right = glm::vec3(worldMatrix[0]);//*glm::vec3(-1, -1, -1);
	m_Up = glm::vec3(worldMatrix[1]);// *glm::vec3(-1, -1, -1);
	m_Front = glm::vec3(worldMatrix[2]) * glm::vec3(-1, -1, -1);

	glm::vec4 vP = (m_inverseViewMatrix * glm::vec4(0, 0, 0, 1));
	m_viewPos = glm::vec3(vP.x, vP.y, vP.z);

	m_projectionViewMatrix = m_projectionMatrix * m_viewMatrix;
}

void Camera::CalculateProjectionMatrix(int screenWidth, int screenHeight)
{
	m_projectionMatrix = glm::perspective(1.0f, (float)screenWidth / (float)screenHeight, NEAR_PLANE, FAR_PLANE);
	m_inversePprojectionMatrix = glm::inverse(m_projectionMatrix);
}

void Camera::Update(float deltaTime)
{
	MouseLook(deltaTime);
	PlayerMovement(deltaTime);
}

void Camera::MouseLook(float deltaTime)
{
	m_xoffset = (float)Input::s_mouseX - m_oldX;
	m_yoffset = (float)Input::s_mouseY - m_oldY;
	m_oldX = (float)Input::s_mouseX;
	m_oldY = (float)Input::s_mouseY;

	if (m_disableMouseLookTimer > 0) {
		m_disableMouseLookTimer--;
		return;
	}

	if (Input::s_showCursor)
		return;

	float yLimit = 1.5f;
	//m_transform.rotation += glm::vec3(m_yoffset, -m_xoffset, 0.0) / glm::vec3(201 - 10);
	//m_transform.rotation.x = std::min(m_transform.rotation.x, -HELL_PI + yLimit);
	//m_transform.rotation.x = std::max(m_transform.rotation.x, -HELL_PI - yLimit);
	
	// if release
	m_transform.rotation += glm::vec3(-m_yoffset, -m_xoffset, 0.0) / glm::vec3(201 - 10);
	m_transform.rotation.x = std::min(m_transform.rotation.x,  yLimit);
	m_transform.rotation.x = std::max(m_transform.rotation.x, - yLimit);

}

void Camera::PlayerMovement(float deltaTime)
{
	float cameraSpeed = 2.5f * deltaTime;
	glm::vec3 Front = m_Front;
	Front.y = 0;
	Front = glm::normalize(Front);

	glm::vec3 Forward = glm::normalize(glm::vec3(Front.x, 0, Front.z));

	float speed = 5;
	
	if (Input::s_keyDown[HELL_KEY_W]) {
		m_transform.position += Forward * deltaTime * speed;
	}
	if (Input::s_keyDown[HELL_KEY_S]) {
		m_transform.position -= Forward * deltaTime * speed;
	}
	/*if (Input::s_keyDown[HELL_KEY_A]) {
		m_transform.position += m_Right * deltaTime * speed;
	}
	if (Input::s_keyDown[HELL_KEY_D]) {
		m_transform.position -= m_Right * deltaTime * speed;
	}*/

	if (Input::s_keyDown[HELL_KEY_A]) {
		m_transform.position -= m_Right * deltaTime * speed;
	}
	if (Input::s_keyDown[HELL_KEY_D]) {
		m_transform.position += m_Right * deltaTime * speed;
	}
}