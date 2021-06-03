#pragma once
#include "Global.h"

struct VTX { // Vertex
	Vector3 vPos;
	Vector4 vColor;
};

// 상수버퍼 용도
struct TTransform {
	Matrix matWorld;
	Matrix matView;
	Matrix matProjection;
};