#pragma once

class Input
{
public: // functions
	static void MouseUpdate(double xpos, double ypos);
	//static void HandleKeypresses(Game* game);
	static void HandleKeypresses();
	//static void HandleKeydowns();

public: // variables
	static int s_mouseX;
	static int s_mouseY;
	static int s_storedMouseX;
	static int s_storedMouseY;
	static int s_mouseX_Editor;
	static int s_mouseY_Editor;
	static bool s_showCursor;

	static bool s_keyPressed[350];
	static bool s_keyDown[350];
	static bool s_keyDownLastFrame[350];

	static bool s_leftMousePressed;
	static bool s_rightMousePressed;
	static bool s_leftMouseDown;
	static bool s_rightMouseDown;
	static bool s_leftMouseDownLastFrame;
	static bool s_rightMouseDownLastFrame;

	static bool s_showBulletDebug;
};
