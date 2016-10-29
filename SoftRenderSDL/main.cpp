#include <windows.h>   // include important windows stuff
#include <cstdio>

#include "GraphicsLib.h"
#include "Model.h"
#include "GMath.h"
#include "Matrix.h"
#include "Camera.h"
#include "Enum.h"

#pragma comment(lib,"SDL2.lib")
#pragma comment(lib,"SDL2main.lib")

#pragma warning(disable:4244) //ȫ���ص�

#define WINDOW_WIDTH 400            //���ڳߴ�
#define WINDOW_HEIGHT 400

Graphics g_graphics;

Model model(Vector3(0, 0, 8));


Camera camera;

State g_game_state = MODEL_TRANSFORM;
RenderState g_render_state = WIREFRAME;

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

void cubeInit()
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
	//model.texture_ = new Texture(path);

	for (int i = 0; i < 24; i++)
	{
		Vector3 v(cube_vertex[i][0], cube_vertex[i][1], cube_vertex[i][2]);
		AColor color(0, colors[i][0], colors[i][1], colors[i][2]);
		Vertex vertex(v, color, uv[i % 4][0], uv[i % 4][1]);
		model.local_vertexes_.push_back(vertex);
		model.trans_vertexes_.push_back(Vertex(v + model.world_positon_, color, uv[i % 4][0], uv[i % 4][1]));
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

//��Ϸѭ��
void GAME_Main(RenderState renderState)
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
		g_graphics.drawwireframe_model(model, remove_vertex_index, remove_triangle_index);
		break;
	}
}

//λͼ����ѭ��
int Draw_Main()
{
	g_graphics.fillSurface();

	//
	GAME_Main(g_render_state);


	AColor color(0, 255, 0, 0);
	g_graphics.drawLine(0, 0, 100, 100, color);


	g_graphics.flipSurface();

	return 1;
}


void close()
{
	g_graphics.close();
}


int main(int argc, char *argv[])
{
	printf("Start Game!\n");

	bool result = g_graphics.initialGraphics(WINDOW_WIDTH, WINDOW_HEIGHT);

	if (!result)
	{
		printf("Failed to initialize!\n");
		return 1;
	}

	cubeInit();

	transform_attribute_init();

	camera.set_wh(WINDOW_WIDTH / WINDOW_HEIGHT);

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


	close();

	return 0;
}
