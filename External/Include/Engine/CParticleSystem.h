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
	
	CStructuredBuffer* m_pParticleBuffer; // ��ƼŬ ������ ����Ǿ��ִ� ����(GPU)
	SharedPtr<CParticleUpdateShader> m_pUpdateShader; // ��ƼŬ Update�� Compute Shader

	Vector4 m_vStartColor;		 // ��ƼŬ ���� ����
	Vector4 m_vEndColor;		 // ��ƼŬ ���� ����
	Vector4 m_vStartScale;		 // ��ƼŬ ���� ũ��
	Vector4 m_vEndScale;		 // ��ƼŬ ���� ũ��
								 
	float m_fStartSpeed;		 // ��ƼŬ ���� �ӵ�>>
	float m_fEndSpeed;			 // ��ƼŬ ���� �ӵ�
								 
	UINT	m_iSpawnCntPerSec;	 // �ʴ� ��ƼŬ ���� ����
	UINT	m_iMaxParticleCount; // ��ƼŬ �ִ� ����
	Vector3 m_vRadius;			 // ��ƼŬ ���� ����
	float	m_fAccTime;			 // �����ð�

public:
	virtual void Start() override;
	virtual void Update() override;
	virtual void FinalUpdate() override;
	virtual void Render() override;

private:
	UINT _CalculateSpawnCount(); // ��ƼŬ ���������� ���� ���ϱ�

public:
	CLONE(CParticleSystem);
	CParticleSystem();
	virtual ~CParticleSystem() override;
};