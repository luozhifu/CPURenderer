#pragma once

//多边形和面的状态
#define POLY4DV1_STATE_ACTIVE	0x0001
#define POLY4DV1_STATE_CLIPPED	0x0002
#define POLY4DV1_STATE_BACKFACE	0x0004


//渲染列表定义
#define RENDERLIST4DV1_MAX_POLYS	32768

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
typedef struct POLYF4DV1_TYP
{
	int state;	//状态信息
	int attr;	//多边形物理属性
	int color;

	POINT4D vlist[3];//三角形顶点
	POINT4D tvlist[3];//转换后的顶点

	POLYF4DV1_TYP *next;
	POLYF4DV1_TYP *prev;
}POLYF4DV1,*POLYF4DV1_PTY;

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

	POLYF4DV1_PTY poly_ptrs[RENDERLIST4DV1_MAX_POLYS];

	POLYF4DV1 poly_data[RENDERLIST4DV1_MAX_POLYS];

	int num_polys;	//渲染列表多边形数量
}RENDERLIST4DV1,*RENDERLIST4DV1_PTR;