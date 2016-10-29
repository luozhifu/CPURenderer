#pragma once

#include "Vector3.h"
#include "GMath.h"

class Vertex
{
public:
	Vertex(const Vector3 &position, const AColor &color, float u, float v) :position_(position), color_(color), u_(u), v_(v) {}

	Vector3 position_;
	Vector3 normal_;
	AColor color_;
	float u_, v_;
};

