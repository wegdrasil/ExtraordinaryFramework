#pragma once
#include "Window.h"
//--------------------------------------------------------------------------------
class QuadDemo : public Window
{
public:
	QuadDemo(HINSTANCE hInstance, WindowSettings *settings) : Window(hInstance, settings) {};

	~QuadDemo() {};

	void Loop();
	
	virtual LRESULT CALLBACK VWindowProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);
};
//--------------------------------------------------------------------------------
