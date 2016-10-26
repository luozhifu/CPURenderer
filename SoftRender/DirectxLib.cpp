#include "DirectxLib.h"

bool DirectX::initialDirectX(HINSTANCE hInstance, HWND hWnd, int width, int height)
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
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
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
		hWnd,               // window associated with device
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
			hWnd,
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

	//创建一个绘制表面
	m_device->CreateOffscreenPlainSurface(width, height, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &m_surface, NULL);
	m_surface->GetDesc(&m_surfacedesc);

	//保存屏幕宽、席、Z缓存区大小
	width_ = width;
	height_ = height;
	buffer_size_ = width * height;

	//zbuffer开辟空间
	zbuffer_ = new float[buffer_size_];


	return true;
}

void DirectX::fillSurface()
{
	m_device->ColorFill(m_surface, NULL, D3DCOLOR_XRGB(0, 0, 0));
	fill(zbuffer_, zbuffer_ + buffer_size_, FLT_MAX);
}

void DirectX::lockSurface()
{
	//创建并初始化锁定区域
	memset(&lockedRect, 0, sizeof(lockedRect));
	m_surface->LockRect(&lockedRect, NULL, D3DLOCK_DISCARD);
}

void DirectX::unlockSurface()
{
	m_surface->UnlockRect();
}

void DirectX::drawPixel(int x, int y, AColor color, float depth)
{
	DWORD* imageData = (DWORD*)lockedRect.pBits;

	int w = lockedRect.Pitch / 4;

	UINT index = y * w + x;
	imageData[index] = ARGB(color.a_,color.r_, color.g_, color.b_);
}

void DirectX::drawLine(int x1, int y1, int x2, int y2, AColor color)
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
			drawPixel(x, y, color);
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
			drawPixel(x, y, color);
			eps += dx;
			if ((eps << 1) >= dy)
			{
				x += ux;
				eps -= dy;
			}
		}
	}
}

void DirectX::flipSurface()
{
	//获得后台缓存
	IDirect3DSurface9 * pBackBuffer = NULL;

	//使用自定义表面填充后台缓存
	m_device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	m_device->StretchRect(m_surface, NULL, pBackBuffer, NULL, D3DTEXF_LINEAR);

	//释放缓存，否则或内存泄露
	pBackBuffer->Release();

	//将交换链中的后台缓存显示
	m_device->Present(0, 0, 0, 0);
}

void DirectX::drawwireframe_model(Model & model)
{
	for (TrangleIndex& v : model.polyindices_)
	{
		Vertex v1 = model.trans_vertexes_[v.indices[0]];
		Vertex v2 = model.trans_vertexes_[v.indices[1]];
		Vertex v3 = model.trans_vertexes_[v.indices[2]];

		AColor color(0, 255, 0, 0);

		drawLine(v1.position_.x, v1.position_.y, v2.position_.x, v2.position_.y, color);
		drawLine(v3.position_.x, v3.position_.y, v2.position_.x, v2.position_.y, color);
		drawLine(v1.position_.x, v1.position_.y, v3.position_.x, v3.position_.y, color);
	}
}
