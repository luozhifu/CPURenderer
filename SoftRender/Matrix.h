#pragma once

#include <cassert>
#include "Vector3.h"
#include "Math.h"

class Matrix
{
public:
	Matrix() :m14(0), m24(0), m34(0), m44(1){}

	void identify();
	void setTranslation(const Vector3& d);
	void setRotate(int axis, float theta);
	void setRotate(const Vector3& axis, float theta);
	void setScale(const Vector3& d);

public:
	float m11, m12, m13, m14;
	float m21, m22, m23, m24;
	float m31, m32, m33, m34;
	float tx, ty, tz,m44;


};

Vector3 operator*(const Vector3& p, const Matrix &m);

Matrix operator*(const Matrix& a, const Matrix &m);