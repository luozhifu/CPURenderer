#pragma once
class Vector3
{
public:
	Vector3(float x, float y, float z, float w = 1) :x(x), y(y), z(z), w(w) {}
	Vector3() :x(), y(), z(), w(1) {}
	Vector3(const Vector3& v) :x(v.x), y(v.y), z(v.z), w(v.w) {}

	Vector3 operator+(const Vector3 &v)const{ return Vector3(x + v.x, y + v.y, z + v.z); }

	Vector3 operator-(const Vector3 &v)const{ return Vector3(x - v.x, y - v.y, z - v.z);}

	union {

		float M[4];
		struct {
			float x,y,z,w;
		};
	};
};

inline float operator*(const Vector3& v, const Vector3& v1)
{
	return v1.x*v.x + v1.y*v.y + v1.z*v.z;
}

inline Vector3 operator*(float f, const Vector3& v1)
{
	return Vector3(v1.x*f,v1.y*f , v1.z*f);
}

inline Vector3 cross_product(const Vector3 &v1, const Vector3 &v2)
{
	return Vector3(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}