#include <windows.h>
#include <d3d9.h>

#pragma comment(lib, "d3d9.lib")

#include "util.h"


/*常量定义*/
#define WINDOW_WIDTH      635         //窗口尺寸
#define WINDOW_HEIGHT     480


/*变量定义*/
HWND m_hwnd;

IDirect3DDevice9* m_device;			//d3d设备
IDirect3DSurface9* m_surface;		//directx表面
D3DSURFACE_DESC m_surfacedesc;   //表面描述

D3DLOCKED_RECT lockedRect;

DWORD*	m_imageData;		//纹理对象


CAM4DV1			cam;				//单个相机
RENDERLIST4DV1 rend_list;			//单个渲染列表
POLYF4DV1 poly1;					//单个多边形
POINT4D	poly1_pos = { 0,0,100,1 };	//多边形世界坐标


LRESULT CALLBACK sWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
		//draw();
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		}
		break;
	case WM_DESTROY:

		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void setPixels(UINT x, UINT y, UINT color)
{
	UINT index = y * lockedRect.Pitch/4 + x;
	m_imageData[index] = color;
}

void drawLine(int x1, int y1, int x2, int y2, UINT color)
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	int ux = ((dx > 0) << 1) - 1;//x的增量方向,取1或-1
	int uy = ((dy > 0) << 1) - 1;//y的增量方向,取1或-1

	dx = abs(dx);
	dy = abs(dy);

	int x = x1, y = y1;
	int eps = 0;

	//线段靠近x轴
	if (dx > dy)
	{
		for (x = x1; x != x2 + ux; x += ux)
		{
			setPixels(x, y, color);
			eps += dy;
			if ((eps << 1) >= dx)
			{
				y += uy;
				eps -= dx;
			}
		}
	}
	else
	{
		for (y = y1; y != y2 + uy; y += uy)
		{
			setPixels(x, y, color);
			eps += dx;
			if ((eps << 1) >= dy)
			{
				x += ux;
				eps -= dy;
			}
		}
	}
}

void draw()
{
	drawLine(0, 0, 100, 100, 0xffff0000);
}

bool Game_Init()
{
	//初始化directx
	D3DDEVTYPE deviceType = D3DDEVTYPE_HAL;

	HRESULT hr = 0;

	// Step 1: Create the IDirect3D9 object.

	IDirect3D9* d3d9 = 0;
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d9)
	{
		::MessageBox(0, "Direct3DCreate9() - FAILED", 0, 0);
		return false;
	}

	// Step 2: Check for hardware vp.

	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);

	int vp = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// Step 3: Fill out the D3DPRESENT_PARAMETERS structure.

	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth = WINDOW_WIDTH;
	d3dpp.BackBufferHeight = WINDOW_HEIGHT;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = m_hwnd;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// Step 4: Create the device.

	hr = d3d9->CreateDevice(
		D3DADAPTER_DEFAULT, // primary adapter
		deviceType,         // device type
		m_hwnd,               // window associated with device
		vp,                 // vertex processing
		&d3dpp,             // present parameters
		&m_device);            // return created device

	if (FAILED(hr))
	{
		// try again using a 16-bit depth buffer
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

		hr = d3d9->CreateDevice(
			D3DADAPTER_DEFAULT,
			deviceType,
			m_hwnd,
			vp,
			&d3dpp,
			&m_device);

		if (FAILED(hr))
		{
			d3d9->Release(); // done with d3d9 object
			::MessageBox(0, "CreateDevice() - FAILED", 0, 0);
			return false;
		}
	}

	d3d9->Release(); // done with d3d9 object

	m_device->CreateOffscreenPlainSurface(WINDOW_WIDTH, WINDOW_HEIGHT, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &m_surface, NULL);
	m_surface->GetDesc(&m_surfacedesc);

	//初始化单个多边形
	poly1.vlist[0].x = 0;
	poly1.vlist[0].x = 50;
	poly1.vlist[0].x = 0;
	poly1.vlist[0].x = 1;

	poly1.vlist[1].x = 50;
	poly1.vlist[1].x = -50;
	poly1.vlist[1].x = 0;
	poly1.vlist[1].x = 1;

	poly1.vlist[2].x = -50;
	poly1.vlist[2].x = -50;
	poly1.vlist[2].x = 0;
	poly1.vlist[2].x = 1;

	poly1.next = poly1.prev = nullptr;



	return true;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	WNDCLASSEX winClass;

	winClass.lpszClassName = "DirectSurface";
	winClass.cbSize = sizeof(WNDCLASSEX);
	winClass.style = CS_HREDRAW | CS_VREDRAW;
	winClass.lpfnWndProc = sWndProc;
	winClass.hInstance = hInstance;
	winClass.hIcon = NULL;
	winClass.hIconSm = NULL;
	winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winClass.hbrBackground = NULL;
	winClass.lpszMenuName = NULL;
	winClass.cbClsExtra = 0;
	winClass.cbWndExtra = 0;

	if (!RegisterClassEx(&winClass))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT!"), "error", MB_ICONERROR);
		return 1;
	}

	m_hwnd = CreateWindowEx(NULL,
		"DirectSurface",                    // window class name
		"Draw Surface",            // window caption
		WS_OVERLAPPEDWINDOW,         // window style
		CW_USEDEFAULT,                // initial x position
		CW_USEDEFAULT,                // initial y position
		WINDOW_WIDTH,                        // initial x size
		WINDOW_HEIGHT,                        // initial y size
		NULL,                        // parent window handle
		NULL,                        // window menu handle
		hInstance,                    // program instance handle
		NULL);                        // creation parameters


	Game_Init();

	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);

	MSG    msg;
	ZeroMemory(&msg, sizeof(msg));

	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{

			if (msg.message == WM_QUIT)
				break;

			// translate any accelerator keys
			TranslateMessage(&msg);

			// send the message to the window proc
			DispatchMessage(&msg);
		} // end while


		m_surface->LockRect(&lockedRect, 0, 0);

		m_imageData = (DWORD*)lockedRect.pBits;

		int w = lockedRect.Pitch / 4;

		draw();


		m_surface->UnlockRect();

		//渲染
		m_device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		m_device->BeginScene();

		IDirect3DSurface9 * pBackBuffer = NULL;

		m_device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
		m_device->StretchRect(m_surface, NULL, pBackBuffer, NULL, D3DTEXF_LINEAR);

		m_device->EndScene();
		m_device->Present(0, 0, 0, 0);

		pBackBuffer->Release();
	}
	
	

	//释放资源
	m_device->Release();
	if (m_surface)
		m_surface->Release();

	return 0;
}