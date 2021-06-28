#pragma once
#include "Global.h"

// Binding structure for GPU

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

	Matrix matWorldView;	 // World * View Matrix
	Matrix matWorldViewProj; // World * View * Projection Matrix
};

struct TMaterialData {
	Vector4 vDiffuse; // 분산광
	Vector4 vSpecular;// 반영광
	Vector4 vAmbiant; // 주변광
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
	Vector4 vDiffuse;	// 난반사광
	Vector4 vSpecular;	// 반사광
	Vector4 vEmbient;	// 환경광
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
	float fAccTime;		// 델터 타임의 누적시간
	int iLight2DCount;
	int iLight3DCount;
	int iPadding[2];
};

struct TTileInfo {
	Vector2 vLeftTop;
	Vector2 vRightBottom;
	Vector2 vTileSize;
	Vector2 vLeftTopUV;	// 타일의 좌상단 좌표 UV
	Vector2 vRightBottomUV; // 타일의 우하단좌표 UV
	Vector2 vTileSizeUV;
};