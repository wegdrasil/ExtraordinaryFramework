#ifndef windowsettings_h
#define windowsettings_h

#include <string>

struct WindowSettings
{
	int m_iHeight;
	int m_iWidth;
	bool m_bFullScreen;
	std::wstring m_sCaption; 
	bool m_bMSAAEnable;
};

#endif