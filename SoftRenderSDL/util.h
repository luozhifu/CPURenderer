#pragma once

//多边形和面的状态
#define POLY4DV1_STATE_ACTIVE	0x0001
#define POLY4DV1_STATE_CLIPPED	0x0002
#define POLY4DV1_STATE_BACKFACE	0x0004

//对象的定义
#define OBJECT4DV1_MAX_VERTICES	1024 //64
#define OBJECT4DV1_MAX_POLYS	1024 //128

//对象的状态
#define OBJECT4DV1_STATE_ACTIVE		0x0001
#define OBJECT4DV1_STATE_VISIBLE	0x0002
#define OBJECT4DV1_STATE_CULLED		0x0004

//渲染列表定义
#define RENDERLIST4DV1_MAX_POLYS	32768

//转换控制标志
#define TRANSFORM_LOCAL_ONLY	0	//执行本地和世界坐标系转换

#define TRANSFORM_LOCAL_TO_TRANS	2	//执行转换到本地顶点列表,仅存储结果到顶点列表


//定义相机旋转顺序
#define CAM_ROT_SEQ_ZYX	4

float cos_look[361];
float sin_look[361];

//3d向量
typedef struct VECTOR3D_TYP
{
	union
	{
		float M[3];
		struct
		{
			float x, y, z;
		};
	};
}	VECTOR3D, POINT3D, *VECTOR3D_PTR, *POINT3D_PTR;

//4d向量
typedef struct VECTOR4D_TYP
{
	union
	{
		float M[4];
		struct
		{
			float x, y, z, w;
		};
	};
}	VECTOR4D,POINT4D,*VECTOR4D_PTR,*POINT4D_PTR;

//多边形渲染列表
typedef struct POLY4DV1_TYP
{
	int state;	//状态信息
	int attr;	//多边形物理属性
	int color;


	POINT4D_PTR vlist;
	int vert[3];
}POLY4DV1, *POLY4DV1_PTR;

//多边形渲染列表
typedef struct POLYF4DV1_TYP
{
	int state;	//状态信息
	int attr;	//多边形物理属性
	int color;

	POINT4D vlist[3];//三角形顶点
	POINT4D tvlist[3];//转换后的顶点

	POLYF4DV1_TYP *next;
	POLYF4DV1_TYP *prev;
}POLYF4DV1,*POLYF4DV1_PTR;

//基于顶点列表和多边形列表的对象
typedef struct OBJECT4DV1_TYP
{
	int id;
	char name[64];

	int state;	//状态信息
	int attr;	//多边形物理属性

	float avg_radius;
	float max_radius;

	POINT4D world_pos;

	VECTOR4D dir;

	VECTOR4D ux,uy,uz;


	int num_vertices;

	POINT4D vlist_local[OBJECT4DV1_MAX_VERTICES];//本地顶点数组
	POINT4D vlist_trans[OBJECT4DV1_MAX_VERTICES];//转换后的顶点数组

	int num_polys;

	POLY4DV1 plist[OBJECT4DV1_MAX_POLYS];

}OBJECT4DV1,*OBJECT4DV1_PTR;

//3d 面
typedef struct PLANE3D_TYP
{
	POINT3D p0;
	VECTOR3D n;
} PLANE3D,*PLANE3D_PTR;

//4x4矩阵
typedef struct MATRIX4X4_TYP
{
	union
	{
		float M[4][4];
		struct
		{
			float M00, M01, M02, M03;
			float M10, M11, M12, M13;
			float M20, M21, M22, M23;
			float M30, M31, M32, M33;
		};
	};
}	MATRIX4X4, *MATRIX4X4_PTR;

//相机版本1
typedef struct CAM4DV1_TYP
{
	int state;
	int attr;

	POINT4D pos;

	VECTOR4D dir;


	VECTOR4D u;
	VECTOR4D v;
	VECTOR4D n;

	VECTOR4D target;

	float view_dist;

	float fov;

	float near_clip_z;
	float far_clip_z;

	PLANE3D rt_clip_plane;
	PLANE3D lt_clip_plane;
	PLANE3D tp_clip_plane;
	PLANE3D bt_clip_plane;

	float viewplane_width;
	float viewplane_height;

	float viewport_width;
	float viewport_height;
	float viewport_center_x;
	float viewport_center_y;

	//比率
	float aspect_ratio;

	MATRIX4X4 mcam;
	MATRIX4X4 mper;
	MATRIX4X4 mscr;


}CAM4DV1,*CAM4DV1_PTR;


typedef struct RENDERLIST4DV1_TYP
{
	int state;
	int attr;

	POLYF4DV1_PTR poly_ptrs[RENDERLIST4DV1_MAX_POLYS];

	POLYF4DV1 poly_data[RENDERLIST4DV1_MAX_POLYS];

	int num_polys;	//渲染列表多边形数量
}RENDERLIST4DV1,*RENDERLIST4DV1_PTR;


inline void VECTOR3D_INITXYZ(VECTOR3D_PTR v, float x, float y, float z)
{
	v->x = x;
	v->y = y;
	v->z = z;
}

inline void VECTOR4D_INITXYZ(VECTOR4D_PTR v, float x,float y,float z)
{
	v->x = x;
	v->y = y;
	v->z = z;
	v->w = 1.0;
}

inline void POINT3D_COPY(POINT3D_PTR vdst, POINT3D_PTR vsrc)
{
	vdst->x = vsrc->x;
	vdst->y = vsrc->y;
	vdst->z = vsrc->z;
}

inline void VECTOR3D_COPY(VECTOR3D_PTR vdst, VECTOR3D_PTR vsrc)
{
	vdst->x = vsrc->x;
	vdst->y = vsrc->y;
	vdst->z = vsrc->z;
}

inline void VECTOR4D_COPY(VECTOR4D_PTR vdst, VECTOR4D_PTR vsrc)
{
	vdst->x = vsrc->x;
	vdst->y = vsrc->y;
	vdst->z = vsrc->z;
	vdst->w = vsrc->w;
}

inline void VECTOR3D_ZERO(VECTOR3D_PTR v)
{
	v->x = v->y = v->z = 0.0;
}

inline void VECTOR4D_ZERO(VECTOR4D_PTR v)
{
	v->x = v->y = v->z = 0.0;
	v->w = 1.0;
}

const MATRIX4X4 IMAT_4X4 = {
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1
};

#define  MAT_IDENTITY_4X4(m) {memcpy((void*)(m),(void*)&IMAT_4X4,sizeof(MATRIX4X4));};

#define  MAT_COPY_4X4(src_mat,dest_mat) {memcpy((void*)(dest_mat),(void*)(src_mat),sizeof(MATRIX4X4));};

#define PI ((float)3.141592654f)

#define EPSILON_ES (float)(1E-5)

#define  DEG_TO_RAD(ang) ((ang)*PI/180.0)

#define  CAM_MODEL_EULER	0x0008

float VECTOR3D_Length(VECTOR3D_PTR va)
{
	return (float)sqrtf(va->x*va->x + va->y*va->y + va->z*va->z);
}

void VECTOR3D_Normalize(VECTOR3D_PTR va, VECTOR3D_PTR vn)
{
	VECTOR3D_ZERO(vn);

	float length = VECTOR3D_Length(va);

	if (length < EPSILON_ES)
		return;

	float length_inv = static_cast<float>(1.0 / length);

	vn->x = va->x * length_inv;
	vn->y = va->y * length_inv;
	vn->y = va->y * length_inv;
}

void PLANE3D_Init(PLANE3D_PTR plane, POINT3D_PTR p0, VECTOR3D_PTR normal, int normalize = 0)
{
	POINT3D_COPY(&plane->p0, p0);

	if (!normalize)
		VECTOR3D_COPY(&plane->n, normal);
	else
		VECTOR3D_Normalize(normal, &plane->n);
}

/** 初始化相机 **/
void Init_CAM4DV1(CAM4DV1_PTR cam,
	int cam_attr,
	POINT4D_PTR cam_pos,
	VECTOR4D_PTR cam_dir,
	POINT4D_PTR cam_target,
	float near_clip_z,
	float far_clip_z,
	float fov,
	float viewport_width,
	float viewport_height)
{
	cam->attr = cam_attr;

	VECTOR4D_COPY(&cam->pos, cam_pos);
	VECTOR4D_COPY(&cam->dir, cam_dir);

	//uvn相机
	VECTOR4D_INITXYZ(&cam->u, 1, 0, 0);//set to +x
	VECTOR4D_INITXYZ(&cam->v, 0, 1, 0);//set to +y
	VECTOR4D_INITXYZ(&cam->n, 0, 0, 1);//set to +z

	if (cam_target)
		VECTOR4D_COPY(&cam->target, cam_target);
	else
		VECTOR4D_ZERO(&cam->target);

	cam->near_clip_z = near_clip_z;
	cam->far_clip_z = far_clip_z;

	cam->viewport_width = viewport_width;
	cam->viewport_height = viewport_height;

	cam->viewport_center_x = (viewport_width - 1) / 2;
	cam->viewport_center_y = (viewport_height - 1) / 2;

	cam->aspect_ratio = viewport_width / viewport_height;

	MAT_IDENTITY_4X4(&cam->mcam);
	MAT_IDENTITY_4X4(&cam->mper);
	MAT_IDENTITY_4X4(&cam->mscr);

	cam->fov = fov;

	cam->viewplane_width = 2.0;
	cam->viewplane_height =static_cast<float>(2.0 / cam->aspect_ratio);

	float tan_fov_div2 =static_cast<float>( tan(DEG_TO_RAD(fov / 2)));

	cam->view_dist =static_cast<float>( (0.5)*(cam->viewplane_width)*tan_fov_div2);

	if (fov == 90.0)
	{
		POINT3D pt_origin;
		VECTOR3D_INITXYZ(&pt_origin, 0, 0, 0);

		VECTOR3D vn;	//面板法线

		VECTOR3D_INITXYZ(&vn, 1, 0, -1);
		PLANE3D_Init(&cam->rt_clip_plane, &pt_origin, &vn, 1);

		VECTOR3D_INITXYZ(&vn, -1, 0, -1);
		PLANE3D_Init(&cam->lt_clip_plane, &pt_origin, &vn, 1);

		VECTOR3D_INITXYZ(&vn, 0, 1, -1);
		PLANE3D_Init(&cam->tp_clip_plane, &pt_origin, &vn, 1);

		VECTOR3D_INITXYZ(&vn, 0, -1, -1);
		PLANE3D_Init(&cam->bt_clip_plane, &pt_origin, &vn, 1);
	}
	else {
		POINT3D pt_origin;
		VECTOR3D_INITXYZ(&pt_origin, 0, 0, 0);

		VECTOR3D vn;	//面板法线

		VECTOR3D_INITXYZ(&vn, cam->view_dist, 0,static_cast<float>( -cam->viewplane_width/2.0));
		PLANE3D_Init(&cam->rt_clip_plane, &pt_origin, &vn, 1);

		VECTOR3D_INITXYZ(&vn, -cam->view_dist, 0, static_cast<float>(-cam->viewplane_width / 2.0));
		PLANE3D_Init(&cam->lt_clip_plane, &pt_origin, &vn, 1);

		VECTOR3D_INITXYZ(&vn, 0,cam->view_dist, static_cast<float>(-cam->viewplane_width / 2.0));
		PLANE3D_Init(&cam->tp_clip_plane, &pt_origin, &vn, 1);

		VECTOR3D_INITXYZ(&vn, 0,-cam->view_dist, static_cast<float>(-cam->viewplane_width / 2.0));
		PLANE3D_Init(&cam->bt_clip_plane, &pt_origin, &vn, 1);
	}
}

void Reset_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list)
{
	rend_list->num_polys = 0;
}



int Insert_POLYF4DV1_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list, POLYF4DV1_PTR poly)
{
	if (rend_list->num_polys >= RENDERLIST4DV1_MAX_POLYS)
		return 0;

	rend_list->poly_ptrs[rend_list->num_polys] = &rend_list->poly_data[rend_list->num_polys];

	memcpy((void*)&rend_list->poly_data[rend_list->num_polys], (void*)poly, sizeof(POLYF4DV1));

	if (rend_list->num_polys == 0)
	{
		rend_list->poly_data[0].next = nullptr;
		rend_list->poly_data[0].prev = nullptr;
	}
	else
	{
		rend_list->poly_data[rend_list->num_polys].next = nullptr;
		rend_list->poly_data[rend_list->num_polys].prev = &rend_list->poly_data[rend_list->num_polys - 1];

		rend_list->poly_data[rend_list->num_polys - 1].next = &rend_list->poly_data[rend_list->num_polys];
	}

	rend_list->num_polys++;

	return 1;
}


void Build_Sin_Cos_Tables()
{
	for (int ang = 0; ang < 360; ang++)
	{
		float theta = (float)ang*PI / (float)180;

		cos_look[ang] =static_cast<float>( cos(theta));
		sin_look[ang] = static_cast<float>(sin(theta));
	}
}

float Fast_Sin(float theta)
{
	theta = fmodf(theta, 360);

	if (theta < 0) theta += 360.0;

	int theta_int = (int)theta;
	float theta_frac = theta - theta_int;

	return (sin_look[theta_int] + theta_frac*(sin_look[theta_int + 1] - sin_look[theta_int]));
}

float Fast_Cos(float theta)
{
	theta = fmodf(theta, 360);

	if (theta < 0) theta += 360.0;

	int theta_int = (int)theta;
	float theta_frac = theta - theta_int;

	return (cos_look[theta_int] + theta_frac*(cos_look[theta_int + 1] - cos_look[theta_int]));
}

void MAT_Init_4X4(MATRIX4X4_PTR ma,
	float m00,float m01,float m02,float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33
	)
{
	ma->M00 = m00, ma->M01 = m01, ma->M02 = m02, ma->M03 = m03;
	ma->M10 = m10, ma->M11 = m11, ma->M12 = m12, ma->M13 = m13;
	ma->M20 = m20, ma->M21 = m21, ma->M22 = m22, ma->M23 = m23;
	ma->M30 = m30, ma->M31 = m31, ma->M32 = m32, ma->M33 = m33;
}

void Build_XYZ_Rotation_MATRIX4X4(int theta_x, float theta_y, int theta_z, MATRIX4X4_PTR mrot)
{
	MATRIX4X4 mx, my, mz, mtmp;
	float sin_theta = 0, cos_theta = 0;
	int rot_seq = 0;		//1 for x,2 for y,4 for z


	MAT_IDENTITY_4X4(mrot);

	if (fabs(theta_x) > EPSILON_ES)
		rot_seq = rot_seq | 1;

	if (fabs(theta_y) > EPSILON_ES)
		rot_seq = rot_seq | 2;

	if (fabs(theta_z) > EPSILON_ES)
		rot_seq = rot_seq | 4;


	switch (rot_seq)
	{
	case 0:
		return;
		break;
	case 2:
		cos_theta = Fast_Cos(theta_y);
		sin_theta = Fast_Sin(theta_y);

		MAT_Init_4X4(&my,
			cos_theta, 0, -sin_theta, 0,
			0, 1, 0, 0,
			sin_theta, 0, cos_theta, 0,
			0, 0, 0, 1
		);

		MAT_COPY_4X4(&my, mrot);
		return;
	}
}

void Mat_Mul_VECTOR4D_4X4(VECTOR4D_PTR va, MATRIX4X4_PTR mb, VECTOR4D_PTR vprod)
{
	for (int col = 0; col < 4; col++)
	{
		float sum = 0;

		for (int row = 0; row < 4; row++)
		{
			sum += (va->M[row] * mb->M[row][col]);
		}

		vprod->M[col] = sum;
	}
}

void Transform_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list, MATRIX4X4_PTR mt, int coord_select)
{
	switch (coord_select)
	{
	case TRANSFORM_LOCAL_ONLY:
		for (int poly = 0; poly < rend_list->num_polys; poly++)
		{
			//获取当前多边形
			POLYF4DV1_PTR curr_poly = rend_list->poly_ptrs[poly];

			if((curr_poly == NULL) || !(curr_poly->state & POLY4DV1_STATE_ACTIVE) ||
				(curr_poly->state & POLY4DV1_STATE_CLIPPED) || 
				(curr_poly->state & POLY4DV1_STATE_BACKFACE))

				continue;

			//开始转换
			for (int vertex = 0; vertex < 3; vertex++)
			{
				POINT4D presult;

				Mat_Mul_VECTOR4D_4X4(&curr_poly->vlist[vertex], mt, &presult);

				VECTOR4D_COPY(&curr_poly->vlist[vertex], &presult);
			}
		}

		break;
	}
}



void VECTOR4D_Add(VECTOR4D_PTR va, VECTOR4D_PTR vb, VECTOR4D_PTR vsum)
{
	vsum->x = va->x + vb->x;
	vsum->y = va->y + vb->y;
	vsum->z = va->z + vb->z;
	vsum->w = 1;
}

void Model_To_World_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list, POINT4D_PTR world_pos,int coord_select=TRANSFORM_LOCAL_TO_TRANS)
{

	if (coord_select == TRANSFORM_LOCAL_TO_TRANS)
	{
		for (int poly = 0; poly < rend_list->num_polys; poly++)
		{
			POLYF4DV1_PTR curr_poly = rend_list->poly_ptrs[poly];

			if ((curr_poly == NULL) || !(curr_poly->state & POLY4DV1_STATE_ACTIVE) ||
				(curr_poly->state & POLY4DV1_STATE_CLIPPED) ||
				(curr_poly->state & POLY4DV1_STATE_BACKFACE))

				continue;

			//开始转换
			for (int vertex = 0; vertex < 3; vertex++)
			{
				VECTOR4D_Add(&curr_poly->vlist[vertex], world_pos, &curr_poly->tvlist[vertex]);
			}
		}
	}
}

void Mat_Mul_4X4(MATRIX4X4_PTR ma, MATRIX4X4_PTR mb, MATRIX4X4_PTR mprod)
{
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			float sum = 0;
			for (int index = 0; index < 4; index++)
			{
				sum += (ma->M[row][index] * mb->M[index][col]);
			}

			mprod->M[row][col] = sum;
		}
	}
}

void Build_CAM4DV1_Matrix_Euler(CAM4DV1_PTR cam, int cam_rot_seq)
{


	MATRIX4X4 mt_inv,
		mx_inv,
		my_inv,
		mz_inv,
		mrot,
		mtmp;


	//创建相机逆矩阵
	MAT_Init_4X4(&mt_inv,
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-cam->pos.x, -cam->pos.y, -cam->pos.z, 1);

	//建立反向旋转顺序

	float theta_x = cam->dir.x;
	float theta_y = cam->dir.y;
	float theta_z = cam->dir.z;


	float cos_theta = Fast_Cos(theta_x);
	float sin_theta = -Fast_Sin(theta_x);

	MAT_Init_4X4(&mx_inv,
		1, 0, 0, 0,
		0, cos_theta, sin_theta, 0,
		0, -sin_theta, cos_theta, 0,
		0, 0, 0, 1);

	cos_theta = Fast_Cos(theta_y);
	sin_theta = -Fast_Sin(theta_y);

	MAT_Init_4X4(&my_inv,
		cos_theta, 0, -sin_theta, 0,
		0, 1, 0, 0,
		sin_theta, 0, cos_theta, 0,
		0, 0, 0, 1);

	cos_theta = Fast_Cos(theta_z);
	sin_theta = -Fast_Sin(theta_z);

	MAT_Init_4X4(&mz_inv,
		cos_theta, 0, sin_theta, 0,
		-sin_theta, cos_theta, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	switch (cam_rot_seq)
	{
	case CAM_ROT_SEQ_ZYX:
		Mat_Mul_4X4(&mz_inv, &my_inv, &mtmp);
		Mat_Mul_4X4(&mtmp, &mx_inv, &mrot);
		break;
	}


	Mat_Mul_4X4(&mt_inv, &mrot, &cam->mcam);
}


void World_To_Camera_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list, CAM4DV1_PTR cam)
{
	for (int poly = 0; poly < rend_list->num_polys; poly++)
	{
		POLYF4DV1_PTR curr_poly = rend_list->poly_ptrs[poly];

		if ((curr_poly == NULL) || !(curr_poly->state & POLY4DV1_STATE_ACTIVE) ||
			(curr_poly->state & POLY4DV1_STATE_CLIPPED) ||
			(curr_poly->state & POLY4DV1_STATE_BACKFACE))

			continue;

		//开始转换
		for (int vertex = 0; vertex < 3; vertex++)
		{
			POINT4D presult;

			Mat_Mul_VECTOR4D_4X4(&curr_poly->tvlist[vertex], &cam->mcam, &presult);

			VECTOR4D_COPY(&curr_poly->tvlist[vertex], &presult);
		}
	}
}



void Camera_To_Perspective_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list, CAM4DV1_PTR cam)
{
	for (int poly = 0; poly < rend_list->num_polys; poly++)
	{
		POLYF4DV1_PTR curr_poly = rend_list->poly_ptrs[poly];

		if ((curr_poly == NULL) || !(curr_poly->state & POLY4DV1_STATE_ACTIVE) ||
			(curr_poly->state & POLY4DV1_STATE_CLIPPED) ||
			(curr_poly->state & POLY4DV1_STATE_BACKFACE))

			continue;

		//开始转换
		for (int vertex = 0; vertex < 3; vertex++)
		{
			float z = curr_poly->tvlist[vertex].z;

			curr_poly->tvlist[vertex].x = cam->view_dist * curr_poly->tvlist[vertex].x / z;
			curr_poly->tvlist[vertex].y = cam->view_dist * curr_poly->tvlist[vertex].y * cam->aspect_ratio/z;
		}
	}
}



void Perspective_To_Screen_RENDERLIST4DV1(RENDERLIST4DV1* rend_list, CAM4DV1* cam)
{
	for (int poly = 0; poly < rend_list->num_polys; poly++)
	{
		POLYF4DV1_PTR curr_poly = rend_list->poly_ptrs[poly];

		if ((curr_poly == NULL) || !(curr_poly->state & POLY4DV1_STATE_ACTIVE) ||
			(curr_poly->state & POLY4DV1_STATE_CLIPPED) ||
			(curr_poly->state & POLY4DV1_STATE_BACKFACE))
			continue;

		float alpha = (0.5*cam->viewport_width - 0.5);
		float beta = (0.5*cam->viewport_height - 0.5);


		//开始转换
		for (int vertex = 0; vertex < 3; vertex++)
		{
			curr_poly->tvlist[vertex].x = alpha + alpha * curr_poly->tvlist[vertex].x;
			curr_poly->tvlist[vertex].y = beta  - beta  * curr_poly->tvlist[vertex].y;
		}
	}
}


void setPixels(int x, int y, UINT color, UINT* pixels, int lpitch)
{
	int index = (int)(lpitch * y + x);

	pixels[index] = color;
}

void drawLine(int x1, int y1, int x2, int y2, UINT color, UINT* pixels, int lpitch)
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
			setPixels(x, y, color, pixels, lpitch);
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
			setPixels(x, y, color, pixels, lpitch);
			eps += dx;
			if ((eps << 1) >= dy)
			{
				x += ux;
				eps -= dy;
			}
		}
	}
}

void Draw_RENDERLIST4DV1_Wire16(RENDERLIST4DV1_PTR rend_list, UINT* pixels, int lpitch)
{
	for (int poly = 0; poly < rend_list->num_polys; poly++)
	{
		POLYF4DV1_PTR curr_poly = rend_list->poly_ptrs[poly];

		if ((curr_poly == NULL) || !(curr_poly->state & POLY4DV1_STATE_ACTIVE) ||
			(curr_poly->state & POLY4DV1_STATE_CLIPPED) ||
			(curr_poly->state & POLY4DV1_STATE_BACKFACE))
			continue;

		drawLine(curr_poly->tvlist[0].x,
			curr_poly->tvlist[0].y,
			curr_poly->tvlist[1].x,
			curr_poly->tvlist[1].y,
			curr_poly->color,
			pixels, lpitch);

		drawLine(curr_poly->tvlist[1].x,
			curr_poly->tvlist[1].y,
			curr_poly->tvlist[2].x,
			curr_poly->tvlist[2].y,
			curr_poly->color,
			pixels, lpitch);

		drawLine(curr_poly->tvlist[2].x,
			curr_poly->tvlist[2].y,
			curr_poly->tvlist[0].x,
			curr_poly->tvlist[0].y,
			curr_poly->color,
			pixels, lpitch);
	}
}

char * Get_Line_PLG(char* buffer, int maxlength, FILE *fp)
{
	int index = 0;
	int length = 0;

	//解析循环
	while (1)
	{
		//读取下一行
		if (!fgets(buffer, maxlength, fp))
			return NULL;

		//清除空白
		for (length = strlen(buffer), index = 0; isspace(buffer[index]); index++);

		if (index >= length || buffer[index] == '#')
			continue;

		return &buffer[index];
	}
}

int Load_OBJECT4DV1_PLG(OBJECT4DV1_PTR obj,
	char *filename,
	VECTOR4D_PTR scale,
	VECTOR4D_PTR pos,
	VECTOR4D_PTR rot)
{

	FILE *fp;
	char buffer[256];

	char *token_string;

	//第一步，清理对象和初始化
	memset(obj, 0, sizeof(OBJECT4DV1));

	obj->state = OBJECT4DV1_STATE_ACTIVE | OBJECT4DV1_STATE_VISIBLE;

	//设置对象的位置
	obj->world_pos.x = pos->x;
	obj->world_pos.y = pos->y;
	obj->world_pos.z = pos->z;
	obj->world_pos.w = pos->w;

	//第二步，读取文件
	if (!(fp = fopen(filename, "r")))
	{
		printf("Couldn't open PLG file %s", filename);
		return 0;
	}

	//第三步，获取第一个令牌字符串做为描述
	token_string = Get_Line_PLG(buffer, 255, fp);
	if(!token_string)
	{
		printf("PLG file error with file %s(object descriptor invalid)", filename);
		return 0;
	}

	printf("object Descriptor:%s", token_string);

	//解析出对象信息
	sscanf(token_string, "%s %d %d", obj->name, &obj->num_vertices, &obj->num_polys);

	//第四步，加载顶点列表
	for (int vertex = 0; vertex < obj->num_vertices; vertex++)
	{
		//获取下一个顶点
		if (!(token_string = Get_Line_PLG(buffer, 255, fp)))
		{
			printf("PLG file error with file %s(vertex list invalid)", filename);
			return 0;
		}

		//解析出顶点
		sscanf(token_string, "%f %f %f", 
			&obj->vlist_local[vertex].x, 
			&obj->vlist_local[vertex].y,
			&obj->vlist_local[vertex].z);

		obj->vlist_local[vertex].w = 1;

		//缩放顶点
		obj->vlist_local[vertex].x *= scale->x;
		obj->vlist_local[vertex].y *= scale->y;
		obj->vlist_local[vertex].z *= scale->z;

		printf("\nVertex %d = %f,%f,%f,%f", vertex,
			obj->vlist_local[vertex].x,
			obj->vlist_local[vertex].y,
			obj->vlist_local[vertex].z,
			obj->vlist_local[vertex].w);
	}

	int poly_surface_desc = 0;
	int poly_num_verts = 0; //每个多边形的顶点数 （总是3）

	char tmp_string[8];

	//第五步：加载多边形列表
	for (int poly = 0; poly < obj->num_polys; poly++)
	{
		//获取下一个多边形描述
		if (!(token_string = Get_Line_PLG(buffer, 255, fp)))
		{
			printf("\nPLG file error with file %s(多边形描述 invalid)", filename);
			return 0;
		}

		printf("\nPolygon:%d", poly);

		//每一个顶点列表必须包含3个顶点，
		sscanf(token_string, "%s %d %d %d %d", tmp_string,
			&poly_num_verts,
			&obj->plist[poly].vert[0],
			&obj->plist[poly].vert[1],
			&obj->plist[poly].vert[2]);

		//判断是否是16进制
		if (tmp_string[0] == '0' && toupper(tmp_string[1]) == 'X')
			sscanf(tmp_string, "%x", &poly_surface_desc);
		else
			poly_surface_desc = atoi(tmp_string);

		obj->plist[poly].vlist = obj->vlist_local;


		printf("\nSurface Desc = 0x%.4x,num_verts = %d,vert_indices [%d,%d,%d]", poly_surface_desc,poly_num_verts, 
			obj->plist[poly].vert[0],
			obj->plist[poly].vert[1],
			obj->plist[poly].vert[2]);
	}


	fclose(fp);

	return 1;
}