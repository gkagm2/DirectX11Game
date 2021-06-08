#pragma once
#include "Global.h"

struct VTX { // Vertex
	Vector3 vPos;
	Vector4 vColor;
	Vector2 vUV;
};

// 상수버퍼 용도
struct TTransform {
	Matrix matWorld;
	Matrix matView;
	Matrix matProjection;
};

struct TMaterialData {
	Vector4 vDiffuse;
	Vector4 vSpectrum;
	Vector4 vAmbiant;
};

struct TMaterialParam {
	TMaterialData mtrl;
	int iArr[4];
	float fArr[4];
	Vector2 v2Arr[4];
	Vector4 v4Arr[4];
	Matrix matArr[4];
};