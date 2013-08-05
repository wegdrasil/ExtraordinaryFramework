#pragma once

#include <Windows.h>
#include <string>

#include "WindowSettings.h"

class Window
{
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	std::string m_sClassName;
	
	WindowSettings* m_Settings;

public:
	Window() {}
	Window(HINSTANCE hInstance, WindowSettings *settings);
	//virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void Init();
	void ShutDown();
	HWND GetHWND() { return m_hWnd; }

private:
	void CreateMyWindow();
	HRESULT RegisterWindowClass();
	void HelpMessage(std::string s);
};

