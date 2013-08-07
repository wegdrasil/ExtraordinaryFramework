#pragma once
//--------------------------------------------------------------------------------
#include <Windows.h>
#include <string>
#include "WindowSettings.h"
//--------------------------------------------------------------------------------
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

	// static message handler to put in WNDCLASSEX structure
	static LRESULT CALLBACK StaticWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
private:
	void CreateMyWindow();
	HRESULT RegisterWindowClass();
	
	void HelpMessage(const std::string s);

protected:
	// the real message handler
	virtual LRESULT CALLBACK VWindowProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);	
};
//--------------------------------------------------------------------------------

