#include "GraphicsLib.h"

bool Graphics::initialGraphics(int width, int height)
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		gWindow = SDL_CreateWindow("sdl test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
		if (gWindow == nullptr)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == nullptr)
			{
				printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				gScreenSurface = SDL_GetWindowSurface(gWindow);

				gTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, width, height);

			}
		}
	}

	//保存屏幕宽、席、Z缓存区大小
	width_ = width;
	height_ = height;
	buffer_size_ = width * height;

	//像素开辟空间
	pixels = new UINT[buffer_size_];

	//zbuffer开辟空间
	zbuffer_ = new float[buffer_size_];


	return true;
}


void Graphics::drawPixel(int x, int y, AColor color, float depth)
{
	int index = (int)(width_ * y + x);

	pixels[index] = ARGB(color.a_, color.r_, color.g_, color.b_);
}

void Graphics::drawLine(int x1, int y1, int x2, int y2, AColor color)
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


void Graphics::fillSurface()
{
	//清屏
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	//清空像素
	memset(pixels, 0xFF0000, buffer_size_ * sizeof(Uint32));
}

void Graphics::flipSurface()
{
	SDL_UpdateTexture(gTexture, NULL, pixels, width_ * sizeof(Uint32));

	//渲染贴图到屏幕
	SDL_RenderCopy(gRenderer, gTexture, nullptr, nullptr);

	//显示
	SDL_RenderPresent(gRenderer);
}

void Graphics::drawwireframe_model(Model & model, const set<int> remove_vertex_index, const set<int> remove_triangle_index)
{
	for (int index = 0; index < model.polyindices_.size(); index++)
	{
		TrangleIndex v = model.polyindices_[index];
		//剔除或不在视锥内
		if (remove_triangle_index.count(index) || is_out(v, remove_vertex_index))
			continue;

		Vertex v1 = model.trans_vertexes_[v.indices[0]];
		Vertex v2 = model.trans_vertexes_[v.indices[1]];
		Vertex v3 = model.trans_vertexes_[v.indices[2]];

		AColor color(0, 255, 0, 0);

		drawLine(v1.position_.x, v1.position_.y, v2.position_.x, v2.position_.y, color);
		drawLine(v3.position_.x, v3.position_.y, v2.position_.x, v2.position_.y, color);
		drawLine(v1.position_.x, v1.position_.y, v3.position_.x, v3.position_.y, color);
	}
}

bool Graphics::is_out(TrangleIndex & triangle, const set<int>& remove_vertex_index)
{
	for (auto i : triangle.indices)
	{
		if (remove_vertex_index.count(i))
			return true;
	}

	return false;
}


void Graphics::close()
{
	SDL_DestroyTexture(gTexture);
	gTexture = nullptr;

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	gRenderer = nullptr;

	SDL_Quit();


	delete[] pixels;
	delete[] zbuffer_;
}
