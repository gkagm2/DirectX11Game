#pragma once
#include "Global.h"

// Binding structure for GPU

struct VTX { // Vertex
	Vector3 vPos;
	Vector4 vColor;
	Vector2 vUV;

	Vector3 vTangent; // 표면의 접선 벡터   (평행)
	Vector3 vNormal;  // 표면의 법선 벡터   (수직)
	Vector3 vBinormal;// 표면의 종법선 벡터 (평행)

	Vector4 vWeights;	// 3D Animation 본 가중치
	Vector4 vIndices;	// 3D Animation 본 인덱스
};

/////////////////////////////
// Strctured Buffer Usage

struct TRay {
	Vector3 vStartPos;
	Vector3 vDir;
};

struct TRaycastOut {
	Vector2 vUV;
	int iDist;
	int bSuccess;
};

struct TTileInfo {
	int idx;
	int iTilePadding[3];
};


// 추가시 GUI쪽도 추가해주기.
struct TLightColor {
	Vector4 vDiffuse;	// 난반사광
	Vector4 vSpecular;	// 반사광
	Vector4 vEmbient;	// 환경광
};

struct TLightInfo {
	int idx; // 고유 번호
	TLightColor tColor;

	Vector4 vLightPos;
	Vector4 vLightDir;
	E_LightType eLightType;
	float fRange;
	float fAngle_Radian; // 라디안 단위의 각도
};

struct TParticle {
	Vector3 vWorldPos;
	Vector3 vWorldDir;

	float fCurTime;
	float fMaxTime;
	int iAlive;

	float fFallTime;
	float fFallStartY;
	float fSpeed;
	int iPadding[2];
};





//////////////////////////////
// Const Buffer Usage

struct TTransform {
	Matrix matWorld;
	Matrix matView;
	Matrix matProjection;

	Matrix matWorldInv;
	Matrix matViewInv;
	Matrix matProjInv;

	Matrix matWorldView;	 // World * View Matrix
	Matrix matWorldViewProj; // World * View * Projection Matrix
};

struct TRectTransform {
	Matrix matWorld;
	Matrix matView;
	Matrix matProjection;
};

struct TMaterialData {
	Vector4 vDiffuse; // 분산광
	Vector4 vSpecular;// 반영광
	Vector4 vAmbiant; // 주변광
	Vector4 vEmission;
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

struct TParticleData {
	Vector3 vWorldPos;
	Vector4 vStartColor;
	Vector4 vEndColor;
	Vector4 vStartScale;
	Vector4 vEndScale;
	
	float fStartSpeed;
	float fEndSpeed;
	
	float fMinLifeTime;
	float fMaxLifeTime;

	UINT iSpawnCntPerSec;
	UINT iMaxParticleCount;
	Vector3 vRadius;
	float fAccTime;

	int iGravityEnable;
	int iShapeType;

	int padding;
};

struct TAnimation2DData {
	Vector2 vLeftTopUV;
	Vector2 vFrameSizeUV;
	Vector2 vOffsetPosUV;
	Vector2 vBaseSizeUV;
	int bIsAnimating2D;
	int padding[3];
};

struct TGlobalConst {
	Vector2 vResolution;
	Vector2 vNoiseResolution; // Noise Texture 해상도
	float fDeltaTime;
	float fAccTime;		// 델터 타임의 누적시간
	int iLight2DCount;
	int iLight3DCount;
};