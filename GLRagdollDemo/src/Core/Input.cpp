//#include "hellpch.h"
#include "Input.h"
#include "keycodes.h"
#include "CoreGL.h"
//#include "Core/CoreImGui.h"
//#include "Game.h"
//#include "Logic/WeaponLogic.h"
#include <algorithm>

int Input::s_mouseX;
int Input::s_mouseY;
int Input::s_storedMouseX;
int Input::s_storedMouseY;
int Input::s_mouseX_Editor;
int Input::s_mouseY_Editor;
bool Input::s_showCursor = false;
bool Input::s_keyPressed[350] = { false };
bool Input::s_keyDown[350] = { false };
bool Input::s_keyDownLastFrame[350] = { false };
bool Input::s_leftMousePressed = false;
bool Input::s_rightMousePressed = false;
bool Input::s_leftMouseDown = false;
bool Input::s_rightMouseDown = false;
bool Input::s_leftMouseDownLastFrame = false;
bool Input::s_rightMouseDownLastFrame = false;
bool Input::s_showBulletDebug = false;
	
void Input::MouseUpdate(double xpos, double ypos)
{
	s_mouseX = xpos;
	s_mouseY = ypos;

	if (s_showCursor) {
		s_mouseX_Editor = xpos;
		s_mouseY_Editor = ypos;
	}
}

void Input::HandleKeypresses()
{
	// Toggle Bullet Debug Drawer
	if (s_keyPressed[HELL_KEY_B])
		s_showBulletDebug = !s_showBulletDebug;

	// Toggle cursor
	if (s_keyPressed[HELL_KEY_M])
		s_showCursor = !s_showCursor;

	// Toggle fullscreen
	if (s_keyPressed[HELL_KEY_F]) {
		CoreGL::ToggleFullScreen();
	}

	// Toggle Bullet Debug Drawer
	//if (s_keyPressed[HELL_KEY_B])
	//	Renderer::m_showBulletDebug = !Renderer::m_showBulletDebug;

	// Toggle ImGui
	/*if (s_keyPressed[HELL_KEY_GRAVE_ACCENT])
	{
		CoreImGui::s_Show = !CoreImGui::s_Show;

		// Always show cursor in level editor, and hide it when you close 
		if (CoreImGui::s_Show)
			s_showCursor = true;
		else
			s_showCursor = false;
	}

	if (Input::s_keyPressed[HELL_KEY_G])
		WeaponLogic::m_singleHanded = !WeaponLogic::m_singleHanded;*/


	// Mouse sensititivity
	/*if (Input::s_keyPressed[HELL_KEY_MINUS])
		Config::MOUSE_SESNSITIVITY -= 10; 
	if (Input::s_keyPressed[HELL_KEY_EQUAL])
		Config::MOUSE_SESNSITIVITY += 10;

	Config::MOUSE_SESNSITIVITY = std::min((int)Config::MOUSE_SESNSITIVITY, 200);
	Config::MOUSE_SESNSITIVITY = std::max((int)Config::MOUSE_SESNSITIVITY, 10);*/
}