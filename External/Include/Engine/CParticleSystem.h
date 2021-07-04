#pragma once
#include "CComponent.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CParticleUpdateShader.h"

class CStructuredBuffer;
class CParticleSystem : public CComponent
{
private:
	SharedPtr<CMesh> m_pMesh;
	SharedPtr<CMaterial> m_pMaterial;
	
	CStructuredBuffer* m_pParticleBuffer; // 파티클 정보가 저장되어있는 버퍼(GPU)
	SharedPtr<CParticleUpdateShader> m_pUpdateShader; // 파티클 Update용 Compute Shader

	Vector4 m_vStartColor;		// 파티클 시작 색상
	Vector4 m_vEndColor;		// 파티클 최종 색상
	Vector4 m_vStartScale;		// 파티클 시작 크기
	Vector4 m_vEndScale;		// 파티클 최종 크기

	float m_fStartSpeed;		// 파티클 시작 속도
	float m_fEndSpeed;			// 파티클 최종 속도

public:
	virtual void Start() override;
	virtual void Update() override;
	virtual void FinalUpdate() override;
	virtual void Render() override;

public:
	CLONE(CParticleSystem);
	CParticleSystem();
	virtual ~CParticleSystem() override;
};