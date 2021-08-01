#pragma once
#include "CComponent.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CTexture.h"
#include "CParticleUpdateShader.h"

class CStructuredBuffer;
class CParticleSystem : public CComponent
{
private:
	SharedPtr<CMesh> m_pMesh;
	SharedPtr<CMaterial> m_pMaterial;

	vector<SharedPtr<CTexture>> m_vecParticleTex;
	UINT m_iTexIdx;
	
	unique_ptr<CStructuredBuffer> m_pParticleBuffer; // 파티클 정보가 저장되어있는 버퍼(GPU)
	SharedPtr<CParticleUpdateShader> m_pUpdateShader; // 파티클 Update용 Compute Shader

	Vector4 m_vStartColor;		 // 파티클 시작 색상
	Vector4 m_vEndColor;		 // 파티클 최종 색상
	Vector4 m_vStartScale;		 // 파티클 시작 크기
	Vector4 m_vEndScale;		 // 파티클 최종 크기
								 
	float	m_fStartSpeed;		 // 파티클 시작 속도>>
	float	m_fEndSpeed;			 // 파티클 최종 속도

	float	m_fMinLifeTime;		 // 파티클 최소 Life
	float	m_fMaxLifeTime;		 // 파티클 최대 Life
								 
	UINT	m_iSpawnCntPerSec;	 // 초당 파티클 생성 개수
	UINT	m_iMaxParticleCount; // 파티클 최대 개수
	Vector3 m_vRadius;			 // 파티클 생성 범위
	float	m_fAccTime;			 // 누적시간

public:
	virtual void Start() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	UINT _CalculateSpawnCount(); // 파티클 생성가능한 갯수 구하기

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CParticleSystem);
	CParticleSystem();
	CParticleSystem(const CParticleSystem& _origin);
	virtual ~CParticleSystem() override;
};