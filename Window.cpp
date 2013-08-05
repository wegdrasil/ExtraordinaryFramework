#include "Window.h"

Window* gWnd;

Window::Window(HINSTANCE hInstance, WindowSettings* settings)
{
	gWnd = this;

	m_hInstance = hInstance; 
	m_sClassName = "ExtraordinaryFrameworkWindowClass";
	m_Settings = settings;
}

//namespace
//{	
//	Window* gWnd = 0;
//}
//
LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return gWnd->VWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT Window::VWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{	
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_MOUSEMOVE:
		
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

void Window::Init()
{
	RegisterWindowClass();
	CreateMyWindow();
}

HRESULT Window::RegisterWindowClass()
{
	WNDCLASSEX wc;
	
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= WindowProc;                
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= m_hInstance;
	wc.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= (LPCWSTR)m_sClassName.c_str();
	wc.hIconSm			= LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc))
	{
		HelpMessage("RegisterClassEx: Fail");
		return E_FAIL;
	}
	return TRUE;
}

void Window::CreateMyWindow()
{
	DWORD GetMyWindowStyle;
	if(!m_Settings->m_bFullScreen)
		GetMyWindowStyle =  WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;	
	else
		GetMyWindowStyle =  WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	

	m_hWnd = CreateWindowEx(
			NULL,
			(LPCWSTR)m_sClassName.c_str(),
			(LPWSTR)m_Settings->m_sCaption.c_str(),
			GetMyWindowStyle,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			m_Settings->m_iWidth,
			m_Settings->m_iHeight,
			NULL, NULL,
			m_hInstance,
			NULL);

	if(!m_hWnd)
		HelpMessage("CreateWindowEx: Fail");

	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);
}

//Destroys WINAPI window and releases handler.
void Window::ShutDown()
{
	if(m_hWnd)
		DestroyWindow(m_hWnd);
	m_hWnd = nullptr;
}

//Displays a modal dialog box. 
void Window::HelpMessage(const std::string s)
{
	MessageBox(m_hWnd, (LPCWSTR)s.c_str(), L"HelpMessage", MB_OK);
}