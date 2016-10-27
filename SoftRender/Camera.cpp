#include "Camera.h"

void Camera::set_position(const Matrix & m)
{
	world_position_ = world_position_ * m;
}

void Camera::set_lookAt(const Vector3 & rotate_angle)
{
	look_at_theta_ = look_at_theta_ + rotate_angle;
}

void Camera::viewtransform(vector<Vertex>& vertexes)
{
	Matrix viewmove_matrix;
	viewmove_matrix.identify();
	viewmove_matrix.setTranslation(-1 * world_position_);

	Matrix viewrotateX_matrix;
	viewrotateX_matrix.setRotate(1, look_at_theta_.y);

	Matrix viewrotateY_matrix;
	viewrotateY_matrix.setRotate(2, look_at_theta_.x);

	Matrix viewtransform_matrix = viewmove_matrix * viewrotateX_matrix * viewrotateY_matrix;

	for (auto& v : vertexes)
	{
		v.position_ = v.position_ * viewtransform_matrix;
	}
}
//×ª»»µ½cvv²Ã¼ô¿Õ¼ä
void Camera::canonicalViewvolume(vector<Vertex>& vertexes)
{
	Matrix cvv_matrix;
	cvv_matrix.identify();
	cvv_matrix.m11 = Tan(angle_ / 2) / wh_;
	cvv_matrix.m22 = Tan(angle_ / 2);
	cvv_matrix.m33 =(far_ + near_) /(far_ - near_);
	cvv_matrix.tz = 2 * near_*far_ / (near_ - far_);
	cvv_matrix.m44 = 0;
	cvv_matrix.m34 = 1;

	for (auto& v : vertexes)
	{
		v.position_ = v.position_ * cvv_matrix;
	}
}

bool Camera::is_back(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3)
{
	Vector3 vec13 = v3 - v1;
	Vector3 vec12 = v2 - v1;
	Vector3 vv = cross_product(vec13, vec12);
	return (vv*v1)>0;
}

int Camera::transform_check_cvv(const Vector3 &v)
{
	int check = 0;

	if (v.z < -1.0f) check |= 1;
	if (v.z > 1.0f)  check |= 2;
	if (v.x < -1.0f) check |= 4;
	if (v.x > 1.0f)  check |= 8;
	if (v.y < -1.0f) check |= 16;
	if (v.y > 1.0f)  check |= 32;

	return check;
}
