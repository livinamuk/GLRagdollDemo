#pragma once
#include "Header.h"
#include "Camera.h"
#include "Renderer/Renderer.h"

	class CoreGL
	{
	public: // functions
		static void InitGL(int windowWidth, int windowHeight);
		static void ProcessInput();
		static void OnUpdate();
		static void SwapBuffersAndPollEvents();
		static double GetGLTime();
		static bool IsRunning();
		static void Terminate();
		static void SetVSync(bool enabled);

		static void ToggleFullScreen();

		static void SetFullscreen(bool fullscreen);
		static bool IsFullscreen(void);

		static void CheckGLError_(const char* file, int line);
		static void ClearDefaultFrameBufferToBlack();
		
		static void SetCamera(Camera* camera);

		//static void ToggleFullscreen();

	public: // static variables
		static GLFWwindow* s_window;
		static GLFWmonitor* s_monitor;
		static int s_windowWidth;
		static int s_windowHeight;

	private:
		static int windowedWidth;
		static int windowedHeight;
		static int windowedPosX;
		static int windowedPosY;
		static Camera* p_camera;
	};
