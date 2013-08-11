#pragma once
#include <d3d11.h>
//--------------------------------------------------------------------------------

template<typename T>
class ConstBuffer
{
public:
	ID3D11Buffer* mConstBuffer;
	bool mInit;
	
	ConstBuffer()/*: mInit(false), mConstBuffer(nullptr)*/
	{
	}

	~ConstBuffer()
	{
		if(mConstBuffer)
		{
			mConstBuffer ->Release();
			mConstBuffer = nullptr;
		}
	}

	T data;

	ID3D11Buffer* GetBuffer()
	{
		return mConstBuffer;
	}

	HRESULT InitConstBuffer(ID3D11Device* dev)
	{
		HRESULT hr = S_OK;
			
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
			
		desc.Usage	             = D3D11_USAGE_DYNAMIC;
		desc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags           = 0;
		desc.ByteWidth           = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));
		desc.StructureByteStride = 0;

		hr = dev ->CreateBuffer(&desc, 0, &mConstBuffer);
			
		if(hr != E_FAIL)
			mInit = true;

		return hr;
	}

	void UpdateConstBuffer(ID3D11DeviceContext* ctx)
	{
		_ASSERT(mInit);

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ctx ->Map(mConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		CopyMemory(mappedResource.pData, &data, sizeof(T));
		ctx ->Unmap(mConstBuffer, 0);
	}
	
	ConstBuffer(const ConstBuffer<T>& rhs);
	ConstBuffer<T>& operator=(const ConstBuffer<T>& rhs);
};
//--------------------------------------------------------------------------------
