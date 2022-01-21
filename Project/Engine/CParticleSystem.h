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

	SharedPtr<CTexture> m_pParticleTex;
	
	unique_ptr<CStructuredBuffer> m_pParticleBuffer; // 파티클 정보가 저장되어있는 버퍼(GPU)
	SharedPtr<CParticleUpdateShader> m_pUpdateShader; // 파티클 Update용 Compute Shader

	Vector4 m_vStartColor;		 // 파티클 시작 색상
	Vector4 m_vEndColor;		 // 파티클 최종 색상
	Vector4 m_vStartScale;		 // 파티클 시작 크기
	Vector4 m_vEndScale;		 // 파티클 최종 크기
								 
	float	m_fStartSpeed;		 // 파티클 시작 속도
	float	m_fEndSpeed;		 // 파티클 최종 속도

	float	m_fMinLifeTime;		 // 파티클 최소 Life
	float	m_fMaxLifeTime;		 // 파티클 최대 Life
								 
	UINT	m_iSpawnCntPerSec;	 // 초당 파티클 생성 개수
	UINT	m_iMaxParticleCount; // 파티클 최대 개수
	Vector3 m_vRadius;			 // 파티클 생성 범위
	float	m_fAccTime;			 // 누적시간

	bool	m_bGravityEnable;	 // 중력 적용

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
	Vector3 GetStartScale() { return Vector3(m_vStartScale.x, m_vStartScale.y, m_vStartScale.z); }
	Vector3 GetEndScale() { return Vector3(m_vEndScale.x, m_vEndScale.y, m_vEndScale.z); }
	float GetMinLifeTime() { return m_fMinLifeTime; }
	float GetMaxLifeTime() { return m_fMaxLifeTime; }
	UINT GetSpawnCntPerSec() { return m_iSpawnCntPerSec; }
	UINT GetMaxParticleCount() { return m_iMaxParticleCount; }
	const Vector3& GetRadius() { return m_vRadius; }
	void SetStartSpeed(float _fSpeed) { m_fStartSpeed = _fSpeed; }
	float GetStartSpeed() { return m_fStartSpeed; }
	void SetEndSpeed(float _fSpeed) { m_fEndSpeed = _fSpeed; }
	float GetEndSpeed() { return m_fEndSpeed; }

	SharedPtr<CTexture> GetParticleTexture() { return m_pParticleTex; }
	void SetParticleTexture(SharedPtr<CTexture> _pTexture) { m_pParticleTex = _pTexture; }

	void SetShape(E_ParticleShape _eShape) { m_eShape = _eShape; }
	E_ParticleShape GetShape() { return m_eShape; }

	void SetGravityEnable(bool _bGravityEnable) { m_bGravityEnable = _bGravityEnable; }
	bool IsGravityEnable() { return m_bGravityEnable; }

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