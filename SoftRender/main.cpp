
#include <string>

#include <windows.h>
#include <d3d9.h>

#pragma comment(lib, "d3d9.lib")

using namespace std;

#include "DirectxLib.h"
#include "Model.h"
#include "Math.h"
#include "Matrix.h"
#include "Camera.h"
#include "Enum.h"

/*��������*/
#define WINDOW_WIDTH      800         //���ڳߴ�
#define WINDOW_HEIGHT     800

DirectX m_directx;

Model model(Vector3(0, 0, 8));

Camera camera;

State g_game_state = MODEL_TRANSFORM;
RenderState g_render_state = COLOR;

//��ת����
const float g_rotate_thera = 3.5;

struct ModelTransAttribute
{
	float posZ;
	float rotate_theta;
	float scale;
}g_model;

struct CameraTransAttribute
{
	float posX, posY, posZ;
	float rotateX, rotateY;
}g_camera;


void transform_attribute_init()
{
	g_model.posZ = 0.0f;
	g_model.rotate_theta = 0.0f;
	g_model.scale = 1.0f;

	g_camera.posX = g_camera.posY = g_camera.posZ = 0.0f;
	g_camera.posZ = 0.0f;
	g_camera.rotateX = g_camera.rotateY = 0.0f;
}

LRESULT CALLBACK sWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
		//draw();
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			SendMessage(hwnd, WM_CLOSE, 0, 0);
		}
		else if (wParam == VK_SPACE)
		{
			g_game_state = g_game_state == MODEL_TRANSFORM ? CAMERA_TRANSFORM : MODEL_TRANSFORM;
		}
		else if(wParam == VK_TAB)
		{
			switch (g_render_state)
			{
			case WIREFRAME:
				g_render_state = TEXTURE;
				break;
			case TEXTURE:
				g_render_state = COLOR;
				break;
			case COLOR:
				g_render_state = WIREFRAME;
				break;
			}
			break;
		}

		switch (g_game_state)
		{
		case MODEL_TRANSFORM:
			if (wParam == VK_UP)
			{
				g_model.posZ += 0.1f;
			}
			else if (wParam == VK_DOWN)
			{
				g_model.posZ -= 0.1f;
			}
			else if(wParam == VK_LEFT)
			{
				g_model.rotate_theta += g_rotate_thera;
			}
			else if (wParam == VK_RIGHT)
			{
				g_model.rotate_theta -= g_rotate_thera;
			}
			else if (wParam == VK_NUMPAD1)
			{
				g_model.scale += 0.01f;
			}
			else if (wParam == VK_NUMPAD2)
			{
				g_model.scale -= 0.01f;
			}
			break;
		case CAMERA_TRANSFORM:
			if (wParam == VK_UP)
			{
				g_camera.posY += 0.1f;
			}
			else if (wParam == VK_DOWN)
			{
				g_camera.posY -= 0.1f;
			}
			else if (wParam == VK_LEFT)
			{
				g_camera.posX += 0.1f;
			}
			else if (wParam == VK_RIGHT)
			{
				g_camera.posX -= 0.1f;
			}
			else if (wParam == VK_NUMPAD1)
			{
				g_camera.posZ += 0.1f;
			}
			else if (wParam == VK_NUMPAD2)
			{
				g_camera.posZ -= 0.1f;
			}
			break;

		}
		break;
	case WM_DESTROY:

		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
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

	//Matrix model_move_matrix;
	//model_move_matrix.identify();
	//model_move_matrix.setTranslation(Vector3(0, 0, 100));
	//model.world_positon_ = model.world_positon_ * model_move_matrix;

	////ģ�Ϳռ���ת
	//Matrix model_rotateY_matrix;
	//model_rotateY_matrix.setRotate(Vector3(0, 1, 0), 45);
	//Matrix model_rotateX_matrix;
	//model_rotateX_matrix.setRotate(Vector3(1, 0, 0), 45);
	//Matrix model_rotateZ_matrix;
	//model_rotateZ_matrix.setRotate(Vector3(0, 0, 1), 45);
	//Matrix matrix = model_rotateX_matrix * model_rotateY_matrix * model_rotateZ_matrix;

	//for (int index = 0; index < model.local_vertexes_.size(); index++)
	//{
	//	Vertex& v = model.local_vertexes_[index];
	//	Vector3 modelVector = v.position_ * matrix;
	//	model.trans_vertexes_[index].position_ = modelVector + model.world_positon_;
	//	model.trans_vertexes_[index].normal_ = v.normal_;
	//}

	//Matrix model_rotate_matrix;
	//model_rotate_matrix.setRotate(Vector3(0, 1, 0), 60);

	//for (int index = 0; index < model.local_vertexes_.size(); index++)
	//{
	//	Vertex& v = model.trans_vertexes_[index];
	//	Vector3 modelVector = 0.5f * v.position_;
	//	modelVector = modelVector * model_rotate_matrix;
	//	model.trans_vertexes_[index].position_ = modelVector + model.world_positon_;
	//}

	model.polyindices_.push_back(TrangleIndex(0, 1, 2));
	model.polyindices_.push_back(TrangleIndex(2, 3, 0));//ǰ

	model.polyindices_.push_back(TrangleIndex(4, 5, 6));
	model.polyindices_.push_back(TrangleIndex(6, 7, 4));//��

	model.polyindices_.push_back(TrangleIndex(8, 9, 10));
	model.polyindices_.push_back(TrangleIndex(10, 11, 8));//��

	model.polyindices_.push_back(TrangleIndex(12, 13, 14));
	model.polyindices_.push_back(TrangleIndex(14, 15, 12));//��

	model.polyindices_.push_back(TrangleIndex(16, 17, 18));
	model.polyindices_.push_back(TrangleIndex(18, 19, 16));//��

	model.polyindices_.push_back(TrangleIndex(20, 21, 22));
	model.polyindices_.push_back(TrangleIndex(22, 23, 20));//��
}

void drawcube(RenderState renderState)
{
	//����任

	//ģ�Ϳռ�ƽ��
	Matrix model_move_matrix;
	model_move_matrix.identify();
	model_move_matrix.setTranslation(Vector3(0, 0, g_model.posZ));

	model.world_positon_ = model.world_positon_ * model_move_matrix;

	//��ת
	Matrix model_rotate_matrix;
	model_rotate_matrix.setRotate(Vector3(0, 1, 0), g_model.rotate_theta);

	//����
	Matrix model_scale_matrix;
	model_scale_matrix.setScale(Vector3(g_model.scale, g_model.scale, g_model.scale));

	Matrix model_transform = model_rotate_matrix * model_scale_matrix;

	//ת������������ϵ
	for (int index = 0; index < model.local_vertexes_.size(); index++)
	{
		Vertex& v = model.local_vertexes_[index];

		model.trans_vertexes_[index].position_ = v.position_ * model_transform + model.world_positon_;
	}

	//ȡ���任

	//����ƶ�
	Matrix camera_move_matrix;
	camera_move_matrix.identify();
	camera_move_matrix.setTranslation(Vector3(g_camera.posX, g_camera.posY, g_camera.posZ));

	//�����ת
	camera.set_lookAt(Vector3(g_camera.rotateX, g_camera.rotateY, 0));
	camera.set_position(camera_move_matrix);

	//ת�����������
	camera.viewtransform(model.trans_vertexes_);


	//CVV�ü�
	set<int> remove_triangle_index;
	// 4.3.2 ��¼�޳�������
	int index2 = 0;
	for (auto v : model.polyindices_)
	{
		//�����޳�
		Vector3 v1 = model.trans_vertexes_[v.indices[0]].position_;
		Vector3 v2 = model.trans_vertexes_[v.indices[1]].position_;
		Vector3 v3 = model.trans_vertexes_[v.indices[2]].position_;
		if (camera.is_back(v1, v2, v3))
		{
			remove_triangle_index.insert(index2);
		}
		index2++;
	}
	camera.canonicalViewvolume(model.trans_vertexes_);

	//͸�ӳ���
	for (int index = 0; index < model.trans_vertexes_.size(); index++)
	{
		Vertex& v = model.trans_vertexes_[index];
		v.position_.x /= v.position_.w;
		v.position_.y /= v.position_.w;
		v.position_.z /= v.position_.w;
		v.position_.w = 1 / v.position_.w;
	}

	set<int> remove_vertex_index;
	//��¼�޳���
	for (int index = 0; index < model.trans_vertexes_.size(); index++)
	{
		Vertex& v = model.trans_vertexes_[index];
		if (camera.transform_check_cvv(v.position_))
			remove_vertex_index.insert(index);
	}


	//���ڱ任
	int half_width = WINDOW_WIDTH / 2;
	int half_height = WINDOW_HEIGHT / 2;
	for (int index = 0; index < model.trans_vertexes_.size(); index++)
	{
		Vertex& v = model.trans_vertexes_[index];
		v.position_.x *= half_width;
		v.position_.x += half_width;

		v.position_.y *= half_height;
		v.position_.y = half_height - v.position_.y;
	}

	//�����߿�ģ��
	switch (renderState)
	{
	case WIREFRAME:
		m_directx.drawwireframe_model(model,remove_vertex_index,remove_triangle_index);
		break;
	}
}

HWND Game_Init(HINSTANCE hInstance,int nShowCmd,string name,string title)
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
		return nullptr;
	}

	HWND hwnd = CreateWindowEx(NULL,
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

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);


	//DirectX��ʼ��
	m_directx.initialDirectX(hInstance, hwnd, WINDOW_WIDTH, WINDOW_HEIGHT);

	//Cube��ʼ��
	cube_init();

	transform_attribute_init();

	camera.set_wh(WINDOW_WIDTH / WINDOW_HEIGHT);

	return hwnd;
}

void gameUpdate(HWND hwnd)
{
	//	drawLine(0, 0, 100, 100, 0xffff0000);

	m_directx.fillSurface();
	m_directx.lockSurface();

	//m_directx.drawLine(0, 0, 100, 100, AColor(0, 255, 0, 0));

	drawcube(g_render_state);

	m_directx.unlockSurface();

	m_directx.flipSurface();

//	transform_attribute_init();
}

void gameEnd(string wcName, HINSTANCE hInstance)
{
	UnregisterClass(wcName.c_str(), hInstance);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	//��������
	string windowClassName = "MyWindow";

	string title = "3DRender";

	HWND hwnd = Game_Init(hInstance, iCmdShow, windowClassName, title);

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

		gameUpdate(hwnd);

	}

	//�ͷ���Դ
	gameEnd(windowClassName, hInstance);

	return 0;
}