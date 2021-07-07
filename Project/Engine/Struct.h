#pragma once
#include "Global.h"

// Binding structure for GPU

struct VTX { // Vertex
	Vector3 vPos;
	Vector4 vColor;
	Vector2 vUV;
};

/////////////////////////////
// Strctured Buffer Usage

struct TTileInfo {
	Vector2 vLeftTop;
	Vector2 vRightBottom;
	Vector2 vTileSize;
	Vector2 vLeftTopUV;	// Ÿ���� �»�� ��ǥ UV
	Vector2 vRightBottomUV; // Ÿ���� ���ϴ���ǥ UV
	Vector2 vTileSizeUV;
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

struct TParticle {
	Vector3 vWorldPos;
	Vector3 vWorldDir;

	float fCurTime;
	float fMaxTime;
	int iAlive;
	int iPadding[3];
};


//////////////////////////////
// Const Buffer Usage

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

struct TGlobalConst {
	Vector2 vResolution;
	Vector2 vNoiseResolution; // Noise Texture �ػ�
	float fDeltaTime;
	float fAccTime;		// ���� Ÿ���� �����ð�
	int iLight2DCount;
	int iLight3DCount;
};