#pragma once
#include "CComputeShader.h"

struct TSharedParticleData {
	UINT m_iSpawnCount;			// 생성할 파티클의 최대 개수
	UINT m_iCurActivedCount;	// 현재 활성화되어있는 파티클의 개수
	UINT m_iPad[2];
};

class CParticleUpdateShader : public CComputeShader
{
private:
	CStructuredBuffer* m_pParticleBuffer; // GameObject 의 ParticleSystem 컴포넌트로부터 받아온 파티클 데이터
	
	unique_ptr<CStructuredBuffer> m_pSharedBuffer;	// 파티클 컴퓨트 쉐이더가 자체적으로 보유한 모든 파티클들이 공유하는 자원 (sys mem과 gpu mem - compute shader와 소통 가능)

	Vector4 m_vObjectPos;
	Vector3 m_vRadius;

	float m_fStartSpeed;
	float m_fEndSpeed;
	UINT  m_iSpawnCount; // 이번 프레임에 파티클을 생성 할 개수

public:
	virtual void UpdateData() override;
	virtual void Clear() override;
	virtual void Excute() override;

public:
	void SetParticleBuffer(CStructuredBuffer* _pBuffer) { m_pParticleBuffer = _pBuffer; }
	void SetStartSpeed(float _fSpeed) { m_fStartSpeed = _fSpeed; }
	void SetEndSpeed(float _fSpeed) { m_fEndSpeed = _fSpeed; }
	void SetSpawnCount(UINT _iSpawnCount) { m_iSpawnCount = _iSpawnCount; }
	void SetRadius(const Vector3& _vRadius) { m_vRadius = _vRadius; }
	void SetWorldPos(const Vector3& _vPos) { m_vObjectPos = _vPos; }

public:
	CParticleUpdateShader();
	virtual ~CParticleUpdateShader() override;
};