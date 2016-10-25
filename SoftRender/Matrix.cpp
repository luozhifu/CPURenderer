#include "Matrix.h"
#include "Vector3.h"

void Matrix::identify()
{
	m11 = 1, m12 = 0, m13 = 0;
	m21 = 0, m22 = 1, m23 = 0;
	m31 = 0, m32 = 0, m33 = 1;
	tx =0,ty=0,tz = 0;
}

void Matrix::setTranslation(const Vector3 & d)
{
	tx = d.x_, ty = d.y_, tz = d.z_;
}

Vector3 operator*(const Vector3 & p, const Matrix & m)
{
	return Vector3(
		p.x_*m.m11 + p.y_*m.m21 + p.z_*m.m31 + p.w_ * m.tx,
		p.x_*m.m12 + p.y_*m.m22 + p.z_*m.m32 + p.w_ * m.ty, 
		p.x_*m.m13 + p.y_*m.m23 + p.z_*m.m33 + p.w_ * m.tz, 
		p.x_*m.m14 + p.y_*m.m24 + p.z_*m.m34 + p.w_ * m.m44
	);
}

Matrix operator*(const Matrix & a, const Matrix & m)
{
	Matrix r;


	return r;
}
