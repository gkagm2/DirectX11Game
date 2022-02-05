#pragma once
#include "Global.h"

// Binding structure for GPU

struct VTX { // Vertex
	Vector3 vPos;
	Vector4 vColor;
	Vector2 vUV;

	Vector3 vTangent; // ǥ���� ���� ����   (����)
	Vector3 vNormal;  // ǥ���� ���� ����   (����)
	Vector3 vBinormal;// ǥ���� ������ ���� (����)
};

/////////////////////////////
// Strctured Buffer Usage

struct TTileInfo {
	int idx;
	int iTilePadding[3];
};


// �߰��� GUI�ʵ� �߰����ֱ�.
struct TLightColor {
	Vector4 vDiffuse;	// ���ݻ籤
	Vector4 vSpecular;	// �ݻ籤
	Vector4 vEmbient;	// ȯ�汤
};

struct TLightInfo {
	int idx; // ���� ��ȣ
	TLightColor tColor;

	Vector4 vLightPos;
	Vector4 vLightDir;
	E_LightType eLightType;
	float fRange;
	float fAngle_Radian; // ���� ������ ����
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

	Matrix matWorldView;	 // World * View Matrix
	Matrix matWorldViewProj; // World * View * Projection Matrix
};

struct TRectTransform {
	Matrix matWorld;
	Matrix matView;
	Matrix matProjection;
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
	Vector2 vNoiseResolution; // Noise Texture �ػ�
	float fDeltaTime;
	float fAccTime;		// ���� Ÿ���� �����ð�
	int iLight2DCount;
	int iLight3DCount;
};