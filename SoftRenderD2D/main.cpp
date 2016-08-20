#include <windows.h>
#include <d2d1.h>



#pragma comment(lib, "d2d1.lib")

/*常量定义*/
#define WINDOW_WIDTH      640         //窗口尺寸
#define WINDOW_HEIGHT     480




ID2D1Factory* pD2DFactory = nullptr;
ID2D1HwndRenderTarget* pRenderTarget = nullptr;
ID2D1SolidColorBrush* pBlackBrush = nullptr;


RECT rc;
HWND g_Hwnd;

IWICBitmap pWICBitmap;


void draw()
{
	
	pRenderTarget->BeginDraw();

	pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

	D2D1_SIZE_F size = pBitmap->GetSize();
//	D2D1_POINT_2F upperLeftCorner = D2D1::Point2F(0.f, 0.f);

	pRenderTarget->DrawBitmap(pBitmap);


	HRESULT hr = pRenderTarget->EndDraw();
	if (FAILED(hr))
	{
		MessageBox(g_Hwnd, "Draw failed!", "Error", 0);
		return;
	}
}


LRESULT CALLBACK sWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
		draw();
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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	WNDCLASSEX winClass;

	winClass.lpszClassName = "D2D";
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

	g_Hwnd = CreateWindowEx(NULL,
		"D2D",                    // window class name
		"D2D test",            // window caption
		WS_OVERLAPPEDWINDOW,         // window style
		CW_USEDEFAULT,                // initial x position
		CW_USEDEFAULT,                // initial y position
		WINDOW_WIDTH,                        // initial x size
		WINDOW_HEIGHT,                        // initial y size
		NULL,                        // parent window handle
		NULL,                        // window menu handle
		hInstance,                    // program instance handle
		NULL);                        // creation parameters

	//创建工厂
	HRESULT hr;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
	if (FAILED(hr))
	{
		MessageBox(g_Hwnd, "Create D2D factory failed!", "Error", 0);
		return 1;
	}

	//RECT rc;
	//GetClientRect(g_Hwnd, &rc);

	//hr = pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(g_Hwnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)), &pRenderTarget);

	//if (FAILED(hr))
	//{
	//	MessageBox(g_Hwnd, "Create render target failed!", "Error", 0);
	//	return 1;
	//}

	hr = pD2DFactory->CreateWicBitmapRenderTarget(pWICBitmap, D2D1::RenderTargetProperties(), &pRenderTarget);

	ShowWindow(g_Hwnd, SW_SHOW);
	UpdateWindow(g_Hwnd);

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

	}
	return 0;
}