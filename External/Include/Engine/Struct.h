#pragma once
#include "Global.h"

// Binding structure for GPU

struct VTX { // Vertex
	Vector3 vPos;
	Vector4 vColor;
	Vector2 vUV;
};

// ������� �뵵
struct TTransform {
	Matrix matWorld;
	Matrix matView;
	Matrix matProjection;

	Matrix matWorldView;	 // World * View Matrix
	Matrix matWorldViewProj; // World * View * Projection Matrix
};

struct TMaterialData {
	Vector4 vDiffuse; // �л걤
	Vector4 vSpecular;// �ݿ���
	Vector4 vAmbiant; // �ֺ���
};

struct TMaterialParam {
	TMaterialData mtrl;
	int iArr[4];
	float fArr[4];
	Vector2 v2Arr[4];
	Vector4 v4Arr[4];
	Matrix matArr[4];

	int bTexArr[(UINT)E_ShaderParam::Texture_End - (UINT)E_ShaderParam::Texture_0];
};

struct TAnimation2DData {
	Vector2 vLeftTopUV;
	Vector2 vFrameSizeUV;
	Vector2 vOffsetSizeUV;
	Vector2 vBaseSizeUV;
	int bIsAnimating2D;
	int padding[3];
};

struct TLightColor {
	Vector4 vDiffuse;	// ���ݻ籤
	Vector4 vSpecular;	// �ݻ籤
	Vector4 vEmbient;	// ȯ�汤
};

struct TLightInfo {
	TLightColor tColor;

	Vector4 vLightPos;
	Vector4 vLightDir;
	E_LightType eLightType;
	float fRange;
	float fAngle;
	int iPad;
};

struct TGlobalConst {
	Vector2 vResolution;
	float fDeltaTime;
	float fAccTime;		// �����ð�
	int iLight2DCount;
	int iLight3DCount;
	int iPadding[2];
};