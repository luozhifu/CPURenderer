#pragma once
class Vector3
{
public:
	Vector3(float x, float y, float z, float w = 1) :x_(x), y_(y), z_(z), w_(w) {}
	Vector3() :x_(), y_(), z_(), w_(1) {}
	Vector3(const Vector3& v) :x_(v.x_), y_(v.y_), z_(v.z_), w_(v.w_) {}

	Vector3 operator+(const Vector3&v)const { return Vector3(x_ + v.x_, y_ + v.y_, z_ + v.z_); }

	float x_;
	float y_;
	float z_;
	float w_;
};

