#include <windows.h>   // include important windows stuff
#include <cstdio>
#include "SDL.h"

#include "util.h"

#pragma comment(lib,"SDL2.lib")
#pragma comment(lib,"SDL2main.lib")

#pragma warning(disable:4244) //全部关掉

#define SCREEN_WIDTH 400            //窗口尺寸
#define SCREEN_HEIGHT 400


SDL_Window* gWindow = nullptr;//渲染的窗口


SDL_Surface* gScreenSurface = nullptr;//窗口包含的表面

SDL_Renderer* gRenderer = nullptr;


SDL_Texture* gTexture = nullptr;//加载和显示到屏幕的图像


UINT pixels[SCREEN_WIDTH * SCREEN_HEIGHT];

POINT4D cam_pos = { 0,0,-100,1 };
VECTOR4D cam_dir = { 0,0,0,1 };

CAM4DV1			cam;				//单个相机
RENDERLIST4DV1 rend_list;			//单个渲染列表
POLYF4DV1 poly1;					//单个多边形
POINT4D	poly1_pos = { 0,0,100,1 };	//多边形世界坐标


bool SDL_Init();//启动sdl，创建窗口

void Game_Init();//相机初始化

void close();

int Draw_Main();


void setPixels(int x, int y, UINT color);

int main(int argc, char *argv[])
{
	printf("Start Game!\n");

	if (!SDL_Init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		bool quit = false;

		SDL_Event e;

		while (!quit)
		{
			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
			}

			Draw_Main();
		}
	}

	close();

	return 0;
}


void Game_Init()
{
	//初始化数学引擎
	Build_Sin_Cos_Tables();

	//初始化单个多边形
	poly1.state = POLY4DV1_STATE_ACTIVE;
	poly1.color = 0xFF00FF00;
	poly1.attr = 0;

	poly1.vlist[0].x = 0;
	poly1.vlist[0].y = 50;
	poly1.vlist[0].z = 0;
	poly1.vlist[0].w = 1;

	poly1.vlist[1].x = 50;
	poly1.vlist[1].y = -50;
	poly1.vlist[1].z = 0;
	poly1.vlist[1].w = 1;

	poly1.vlist[2].x = -50;
	poly1.vlist[2].y = -50;
	poly1.vlist[2].z = 0;
	poly1.vlist[2].w = 1;

	poly1.next = poly1.prev = nullptr;

	//初始化相机
	Init_CAM4DV1(&cam,
		CAM_MODEL_EULER,
		&cam_pos,
		&cam_dir,
		NULL,
		50.0,
		500.0,
		90.0,
		SCREEN_WIDTH,
		SCREEN_HEIGHT);
}

bool SDL_Init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		gWindow = SDL_CreateWindow("sdl test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

				gTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);


				Game_Init();

			}
		}
	}

	return success;
}


void close()
{
	SDL_DestroyTexture(gTexture);
	gTexture = nullptr;

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	gRenderer = nullptr;

	SDL_Quit();
}



//游戏循环
void GAME_Main()
{
	static MATRIX4X4 mrot;
	static float ang_y = 0;

	//设置像素点
	//setPixels(10, 10, 0xFF0000);
	//setPixels(20, 20, 0x00FF00);

	//drawLine(50, 50, 100, 100, 0xFF0000);

	//初始化渲染列表
	Reset_RENDERLIST4DV1(&rend_list);

	//多边形插入渲染列表
	Insert_POLYF4DV1_RENDERLIST4DV1(&rend_list, &poly1);

	Build_XYZ_Rotation_MATRIX4X4(0, ang_y, 0, &mrot);

	if (++ang_y > 360.0) 
		ang_y = 0;

	Transform_RENDERLIST4DV1(&rend_list, &mrot, TRANSFORM_LOCAL_ONLY);

	//执行 本地/模型 到世界转换
	Model_To_World_RENDERLIST4DV1(&rend_list, &poly1_pos);

	//生成相机矩阵
	Build_CAM4DV1_Matrix_Euler(&cam, CAM_ROT_SEQ_ZYX);

	//应用世界坐标到相机坐标
	World_To_Camera_RENDERLIST4DV1(&rend_list, &cam);

	//应用相机坐标到投影
	Camera_To_Perspective_RENDERLIST4DV1(&rend_list, &cam);

	//应用屏幕转换
	Perspective_To_Screen_RENDERLIST4DV1(&rend_list, &cam);

	//渲染多边形
	Draw_RENDERLIST4DV1_Wire16(&rend_list,pixels, SCREEN_WIDTH);
}

//位图绘制循环
int Draw_Main()
{
	//清屏
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	//清空像素
	memset(pixels, 0xFF0000, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));

	//
	GAME_Main();

	SDL_UpdateTexture(gTexture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));

	//渲染贴图到屏幕
	SDL_RenderCopy(gRenderer, gTexture, nullptr, nullptr);

	//显示
	SDL_RenderPresent(gRenderer);

	return 1;
}