#pragma once
#include "WindowSettings.h"

//Supported graphics APIs
enum class RendererType
{
	D3D_11
	//OGL_43
	//OGLES_30
};

class IRenderer
{
public:
	virtual ~IRenderer() {};
	
	virtual void Initialize(WindowSettings* windowSettings, HWND handle ) {};
	virtual void Shutdown() = 0;
	virtual void Present() = 0;

protected:
	IRenderer& operator=(const IRenderer&);

};