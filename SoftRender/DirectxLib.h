#pragma once

#include <d3d9.h>
#include <set>

#include "Math.h"
#include "Model.h"
#include "Math.h"

using namespace std;

class DirectX
{
public:


	bool initialDirectX(HINSTANCE hInstance, HWND hWnd, int width, int height);
	void fillSurface();
	void lockSurface();
	void unlockSurface();

	//绘制像素到缓冲区
	void drawPixel(int x, int y, AColor color, float depth = 0.0f);

	void drawLine(int x1, int y1, int x2, int y2, AColor color);

	//缓存冲翻转并显示
	void flipSurface();


	DWORD ARGB(int a, int r, int g, int b) { return DWORD((a << 24) + (r << 16) + (g << 8) + b); }


	//绘制线框模型
	void drawwireframe_model(Model& model,const set<int> remove_vertex_index,const set<int> remove_triangle_index);

	bool is_out(TrangleIndex &triangle, const set<int> &remove_vertex_index);

private:

	IDirect3DDevice9* m_device;			//d3d设备
	IDirect3DSurface9* m_surface;		//directx表面
	D3DSURFACE_DESC m_surfacedesc;   //表面描述

	D3DLOCKED_RECT lockedRect;

	float * zbuffer_;
	int width_;
	int height_;
	int buffer_size_;
};

