
#include <string>

#include <windows.h>
#include <d3d9.h>

#pragma comment(lib, "d3d9.lib")

using namespace std;

#include "DirectxLib.h"
#include "Model.h"
#include "Math.h"
#include "Matrix.h"

/*常量定义*/
#define WINDOW_WIDTH      635         //窗口尺寸
#define WINDOW_HEIGHT     480

DirectX m_directx;

Model model(Vector3(0, 0, 0));

/*变量定义*/
HWND m_hwnd;


D3DLOCKED_RECT lockedRect;

DWORD*	m_imageData;		//纹理对象


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

void cube_init()
{
	int cube_vertex[24][3] = {
		{ -1,1,-1 },{ -1,-1,-1 },{ 1,-1,-1 },{ 1,1,-1 },
		{ 1,1,-1 },{ 1,-1,-1 },{ 1,-1,1 },{ 1,1,1 },
		{ 1,1,1 },{ 1,-1,1 },{ -1,-1,1 },{ -1,1,1 },
		{ -1,1,1 },{ -1,-1,1 },{ -1,-1,-1 },{ -1,1,-1 },
		{ -1,1,1 },{ -1,1,-1 },{ 1,1,-1 },{ 1,1,1 },
		{ -1,-1,-1 },{ -1,-1,1 },{ 1,-1,1 },{ 1,-1,-1 }
	};


	int colors[24][3] = {
		{ 255,0,0 },{ 0,255,0 },{ 0,0,255 },
		{ 255,0,0 },{ 0,255,0 },{ 0,0,255 },
		{ 255,0,0 },{ 0,255,0 },{ 0,0,255 },
		{ 255,0,0 },{ 0,255,0 },{ 0,0,255 },
		{ 255,0,0 },{ 0,255,0 },{ 0,0,255 },
		{ 255,0,0 },{ 0,255,0 },{ 0,0,255 },
		{ 255,0,0 },{ 0,255,0 },{ 0,0,255 },
		{ 255,0,0 },{ 0,255,0 },{ 0,0,255 },
	};


	int uv[4][2] = {
		{ 0,0 },{ 0,1 },{ 1,1 },{ 1,0 },
	};


	string path = "pal5q.jpg";
	model.texture_ = new Texture(path);

	for (int i = 0; i < 24; i++)
	{
		Vector3 v(cube_vertex[i][0], cube_vertex[i][1], cube_vertex[i][2]);
		AColor color(0, colors[i][0], colors[i][1], colors[i][2]);
		Vertex vertex(v, color, uv[i % 4][0], uv[i % 4][1]);
		model.local_vertexes_.push_back(vertex);
		model.trans_vertexes_.push_back(Vertex(v+model.world_positon_,color, uv[i % 4][0], uv[i % 4][1]));
	}

	Matrix model_move_matrix;
	model_move_matrix.identify();
	model_move_matrix.setTranslation(Vector3(0, 0, -5));
	model.world_positon_ = model.world_positon_ * model_move_matrix;

	//模型空间旋转
	Matrix model_rotateY_matrix;
	model_rotateY_matrix.setRotate(Vector3(0, 1, 0), 45);
}

bool Game_Init(HINSTANCE hInstance,int nShowCmd,string name,string title)
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

	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);


	//DirectX初始化
	m_directx.initialDirectX(hInstance, m_hwnd, WINDOW_WIDTH, WINDOW_HEIGHT);

	//Cube初始化
	cube_init();


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
	//创建窗口
	string windowClassName = "MyWindow";

	string title = "3DRender";

	Game_Init(hInstance, iCmdShow, windowClassName, title);

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