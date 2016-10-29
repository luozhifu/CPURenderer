#pragma once

#include <vector>

#include "Vector3.h"
#include "Vertex.h"
#include "Texture.h"

using namespace std;




class TrangleIndex
{
public:
	TrangleIndex(int i, int j, int k) { indices[0] = i; indices[1] = j; indices[2] = k; }
	TrangleIndex() {}

	int indices[3];
};

class Model
{
public:

	Model(Vector3 positon):world_positon_(positon),texture_(nullptr){}

	Vector3 world_positon_;
	vector<Vertex> local_vertexes_;
	vector<Vertex> trans_vertexes_;

	vector<TrangleIndex> polyindices_;

	Texture* texture_;
};

