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
	
	unique_ptr<CStructuredBuffer> m_pParticleBuffer; // ��ƼŬ ������ ����Ǿ��ִ� ����(GPU)
	SharedPtr<CParticleUpdateShader> m_pUpdateShader; // ��ƼŬ Update�� Compute Shader

	Vector4 m_vStartColor;		 // ��ƼŬ ���� ����
	Vector4 m_vEndColor;		 // ��ƼŬ ���� ����
	Vector4 m_vStartScale;		 // ��ƼŬ ���� ũ��
	Vector4 m_vEndScale;		 // ��ƼŬ ���� ũ��
								 
	float	m_fStartSpeed;		 // ��ƼŬ ���� �ӵ�>>
	float	m_fEndSpeed;			 // ��ƼŬ ���� �ӵ�

	float	m_fMinLifeTime;		 // ��ƼŬ �ּ� Life
	float	m_fMaxLifeTime;		 // ��ƼŬ �ִ� Life
								 
	UINT	m_iSpawnCntPerSec;	 // �ʴ� ��ƼŬ ���� ����
	UINT	m_iMaxParticleCount; // ��ƼŬ �ִ� ����
	Vector3 m_vRadius;			 // ��ƼŬ ���� ����
	float	m_fAccTime;			 // �����ð�

	bool	m_bGravityEnable;	 // �߷� ����

	E_ParticleShape m_eShape;

public:
	virtual void Start() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	void SetStartColor(const Vector4 _vColor) { m_vStartColor = _vColor; }
	void SetEndColor(const Vector4 _vColor) { m_vEndColor = _vColor; }
	void SetStartScale(const Vector3 _vScale) {
		m_vStartScale = Vector4(_vScale.x, _vScale.y, _vScale.z, 1.f);
	}
	void SetEndScale(const Vector3 _vScale) {
		m_vEndScale = Vector4(_vScale.x, _vScale.y, _vScale.z, 1.f);
	}
	void SetMinLifeTime(float _fTime) { m_fMinLifeTime = _fTime; }
	void SetMaxLifeTime(float _fTime) { m_fMaxLifeTime = _fTime; }
	void SetSpawnCntPerSec(UINT _iParticleCnt) { m_iSpawnCntPerSec = _iParticleCnt; }
	void SetMaxParticleCount(UINT _iParticleCnt) { m_iMaxParticleCount; }

	void SetRadius(const Vector3 _vRadius) { m_vRadius - _vRadius; }

	const Vector4& GetStartColor() { return m_vStartColor; }
	const Vector4& GetEndColor() { return m_vEndColor; }
	const Vector3& GetStartScale() { return m_vStartScale; }
	const Vector3& GetEndScale() { return m_vEndScale; }
	float GetMinLifeTime() { return m_fMinLifeTime; }
	float GetMaxLifeTime() { return m_fMaxLifeTime; }
	UINT GetSapwnCntPerSec() { return m_iSpawnCntPerSec; }
	UINT GetMaxParticleCount() { return m_iMaxParticleCount; }
	const Vector3& GetRadius() { return m_vRadius; }

	UINT GetParticleTexCnt() { return (UINT)m_vecParticleTex.size(); }
	void SetParticleTexIdx(UINT _idx);

	void SetShape(E_ParticleShape _eShape) { m_eShape = _eShape; }
	E_ParticleShape GetShape() { return m_eShape; }

	void SetGravityEnable(bool _bGravityEnable) { m_bGravityEnable = _bGravityEnable; }
	bool IsGravityEnable() { return m_bGravityEnable; }

private:
	UINT _CalculateSpawnCount(); // ��ƼŬ ���������� ���� ���ϱ�

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CParticleSystem);
	CParticleSystem();
	CParticleSystem(const CParticleSystem& _origin);
	virtual ~CParticleSystem() override;
};