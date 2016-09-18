#include <windows.h>   // include important windows stuff
#include <cstdio>
#include "SDL.h"

#include "util.h"

#pragma comment(lib,"SDL2.lib")
#pragma comment(lib,"SDL2main.lib")

#pragma warning(disable:4244) //ȫ���ص�

#define SCREEN_WIDTH 400            //���ڳߴ�
#define SCREEN_HEIGHT 400


SDL_Window* gWindow = nullptr;//��Ⱦ�Ĵ���


SDL_Surface* gScreenSurface = nullptr;//���ڰ����ı���

SDL_Renderer* gRenderer = nullptr;


SDL_Texture* gTexture = nullptr;//���غ���ʾ����Ļ��ͼ��


UINT pixels[SCREEN_WIDTH * SCREEN_HEIGHT];

POINT4D cam_pos = { 0,0,-100,1 };
VECTOR4D cam_dir = { 0,0,0,1 };

CAM4DV1			cam;				//�������
RENDERLIST4DV1 rend_list;			//������Ⱦ�б�
POLYF4DV1 poly1;					//���������
POINT4D	poly1_pos = { 0,0,100,1 };	//�������������


bool SDL_Init();//����sdl����������

void Game_Init();//�����ʼ��

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
	//��ʼ����ѧ����
	Build_Sin_Cos_Tables();

	//��ʼ�����������
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

	//��ʼ�����
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



//��Ϸѭ��
void GAME_Main()
{
	static MATRIX4X4 mrot;
	static float ang_y = 0;

	//�������ص�
	//setPixels(10, 10, 0xFF0000);
	//setPixels(20, 20, 0x00FF00);

	//drawLine(50, 50, 100, 100, 0xFF0000);

	//��ʼ����Ⱦ�б�
	Reset_RENDERLIST4DV1(&rend_list);

	//����β�����Ⱦ�б�
	Insert_POLYF4DV1_RENDERLIST4DV1(&rend_list, &poly1);

	Build_XYZ_Rotation_MATRIX4X4(0, ang_y, 0, &mrot);

	if (++ang_y > 360.0) 
		ang_y = 0;

	Transform_RENDERLIST4DV1(&rend_list, &mrot, TRANSFORM_LOCAL_ONLY);

	//ִ�� ����/ģ�� ������ת��
	Model_To_World_RENDERLIST4DV1(&rend_list, &poly1_pos);

	//�����������
	Build_CAM4DV1_Matrix_Euler(&cam, CAM_ROT_SEQ_ZYX);

	//Ӧ���������굽�������
	World_To_Camera_RENDERLIST4DV1(&rend_list, &cam);

	//Ӧ��������굽ͶӰ
	Camera_To_Perspective_RENDERLIST4DV1(&rend_list, &cam);

	//Ӧ����Ļת��
	Perspective_To_Screen_RENDERLIST4DV1(&rend_list, &cam);

	//��Ⱦ�����
	Draw_RENDERLIST4DV1_Wire16(&rend_list,pixels, SCREEN_WIDTH);
}

//λͼ����ѭ��
int Draw_Main()
{
	//����
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	//�������
	memset(pixels, 0xFF0000, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));

	//
	GAME_Main();

	SDL_UpdateTexture(gTexture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));

	//��Ⱦ��ͼ����Ļ
	SDL_RenderCopy(gRenderer, gTexture, nullptr, nullptr);

	//��ʾ
	SDL_RenderPresent(gRenderer);

	return 1;
}