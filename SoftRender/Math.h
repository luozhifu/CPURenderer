#pragma once

#include <cmath>

#define PI 3.141592654

class AColor
{
public:
	AColor(int a, int r, int g, int b) :a_(a), r_(r), g_(g), b_(b) {}


	int a_;
	int r_;
	int g_;
	int b_;

};

float Sin(float degree);
float Cos(float degree);
float Tan(float degree);