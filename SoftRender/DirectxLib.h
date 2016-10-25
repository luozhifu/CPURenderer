#pragma once

#include <d3d9.h>


class DirectX
{
public:


	bool initialDirectX(HINSTANCE hInstance, HWND hWnd, int width, int height);


private:

	IDirect3DDevice9* m_device;			//d3d设备
	IDirect3DSurface9* m_surface;		//directx表面
	D3DSURFACE_DESC m_surfacedesc;   //表面描述
};

