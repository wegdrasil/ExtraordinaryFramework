#pragma once
#include <Windows.h>
#include <DirectXMath.h>
//--------------------------------------------------------------------------------
struct Vertex
{
	Vertex() {};
	Vertex(const DirectX::XMFLOAT3& p, const DirectX::XMFLOAT3& n, const DirectX::XMFLOAT2& uv): mPosition(p), mNormal(n), mTexCoord(uv){}
	Vertex(float px, float py, float pz, float nx, float ny, float nz, float u, float v): mPosition(px,py,pz), mNormal(nx,ny,nz), mTexCoord(u,v){}
	
	DirectX::XMFLOAT3 mPosition;	
	DirectX::XMFLOAT3 mNormal;
	DirectX::XMFLOAT2 mTexCoord;
};
//--------------------------------------------------------------------------------
