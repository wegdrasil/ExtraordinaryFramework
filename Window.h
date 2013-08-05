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
	
	void Init();
	void ShutDown();
	HWND GetHWND() { return m_hWnd; }
	HINSTANCE GetHINSTANCE() { return m_hInstance; }

private:
	void CreateMyWindow();
	HRESULT RegisterWindowClass();
	
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	void HelpMessage(const std::string s);

protected:
	virtual LRESULT VWindowProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);	
};

