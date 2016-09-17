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

	POLYF4DV1_PTY poly_ptrs[RENDERLIST4DV1_MAX_POLYS];

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