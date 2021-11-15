#pragma once
#include "CComputeShader.h"

struct TSharedParticleData {
	UINT m_iSpawnCount;			// ������ ��ƼŬ�� �ִ� ����
	UINT m_iCurActivedCount;	// ���� Ȱ��ȭ�Ǿ��ִ� ��ƼŬ�� ����
	UINT m_iPad[2];
};

class CParticleUpdateShader : public CComputeShader
{
private:
	CStructuredBuffer* m_pParticleBuffer; // GameObject �� ParticleSystem ������Ʈ�κ��� �޾ƿ� ��ƼŬ ������
	
	unique_ptr<CStructuredBuffer> m_pSharedBuffer;	// ��ƼŬ ��ǻƮ ���̴��� ��ü������ ������ ��� ��ƼŬ���� �����ϴ� �ڿ� (sys mem�� gpu mem - compute shader�� ���� ����)

	Vector4 m_vObjectPos;
	Vector3 m_vRadius;

	float m_fStartSpeed;
	float m_fEndSpeed;
	float m_fMinLifeTime;
	float m_fMaxLifeTime;
	UINT  m_iSpawnCount; // �̹� �����ӿ� ��ƼŬ�� ���� �� ����

	E_ParticleShape m_eShape; // ��ƼŬ ���
	bool m_bGravityEnable;

public:
	virtual void UpdateData() override;
	virtual void Clear() override;
	virtual void Excute() override;

public:
	void SetParticleBuffer(CStructuredBuffer* _pBuffer) { m_pParticleBuffer = _pBuffer; }
	void SetStartSpeed(float _fSpeed) { m_fStartSpeed = _fSpeed; }
	void SetEndSpeed(float _fSpeed) { m_fEndSpeed = _fSpeed; }
	void SetMinLifeTime(float _fMinLifeTime) { m_fMinLifeTime = _fMinLifeTime; }
	void SetMaxLifeTime(float _fMaxLifeTime) { m_fMaxLifeTime = _fMaxLifeTime; }
	void SetSpawnCount(UINT _iSpawnCount) { m_iSpawnCount = _iSpawnCount; }
	void SetRadius(const Vector3& _vRadius) { m_vRadius = _vRadius; }
	void SetWorldPos(const Vector3& _vPos) { m_vObjectPos = _vPos; }

	void SetShape(E_ParticleShape _eShape) { m_eShape; }

	void SetGravityEnable(bool _bGravityEnable) { m_bGravityEnable = _bGravityEnable; }

public:
	CParticleUpdateShader();
	virtual ~CParticleUpdateShader() override;
};