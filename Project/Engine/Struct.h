#pragma once
#include "Global.h"

struct VTX { // Vertex
	Vector3 vPos;
	Vector4 vColor;
};

// ������� �뵵
struct TTransform {
	Matrix matWorld;
	Matrix matView;
	Matrix matProjection;
};