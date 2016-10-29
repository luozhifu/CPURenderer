#pragma once

#include <d3d9.h>
#include <set>

#include "Math.h"
#include "Model.h"
#include "Math.h"

#include "SDL.h"

using namespace std;

class Graphics
{
public:


	bool initialGraphics(int width, int height);
	void fillSurface();

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

	SDL_Window* gWindow = nullptr;//渲染的窗口

	SDL_Surface* gScreenSurface = nullptr;//窗口包含的表面

	SDL_Renderer* gRenderer = nullptr;


	SDL_Texture* gTexture = nullptr;//加载和显示到屏幕的图像

	/*显示的像素*/
	UINT* pixels;

	float * zbuffer_;
	int width_;
	int height_;
	int buffer_size_;
public:
	void close();
};

