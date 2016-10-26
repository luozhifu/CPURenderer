#pragma once

#include <vector>
#include "Vertex.h"
#include "Matrix.h"
#include "Vector3.h"

using namespace std;

class Camera
{
public:
	Camera() :world_position_(),look_at_theta_(Vector3(0,0,0)),angle_(90),near_(1),far_(10000){}
	
	void set_position(const Matrix& m);
	void set_lookAt(const Vector3& rotate_angle);

	void set_wh(float wh) { wh_ = wh; }

	void viewtransform(vector<Vertex>& vertexes);

	void canonicalViewvolume(vector<Vertex>& vertexes);

private:
	Vector3 world_position_;
	Vector3 look_at_theta_;

	float angle_;//Y轴方向视角度数
	float wh_;//屏幕宽高比
	float near_;
	float far_;
};

