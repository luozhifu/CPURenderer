#pragma once

#include <d3d9.h>


class DirectX
{
public:


	bool initialDirectX(HINSTANCE hInstance, HWND hWnd, int width, int height);


private:

	IDirect3DDevice9* m_device;			//d3d�豸
	IDirect3DSurface9* m_surface;		//directx����
	D3DSURFACE_DESC m_surfacedesc;   //��������
};

