#include "pch.h"
#include "CBulletBouncingParticle_bu.h"

CBulletBouncingParticle_bu::CBulletBouncingParticle_bu() :
	CScript((UINT)SCRIPT_TYPE::BULLETBOUNCINGPARTICLE_BU),
	m_vBouncingDir{},
	m_fSpeed{ 4.f },
	m_fMaxLifeTime{ 1.f },
	m_fLifeTime{ 0.f }
{
}

void CBulletBouncingParticle_bu::Start()
{
	float fRandom = (rand() % 6) * 0.1f + 0.4f;
	m_fMaxLifeTime = fRandom;
}

void CBulletBouncingParticle_bu::Update()
{
	m_fLifeTime += DT;
	if (m_fLifeTime > m_fMaxLifeTime)
		CObject::DestroyGameObjectEvn(GetGameObject());

	Vector3 vPos = Transform()->GetLocalPosition();
	Transform()->SetLocalPosition(m_vBouncingDir * DT * m_fSpeed + vPos);
}

void CBulletBouncingParticle_bu::SetParticle(const Vector3& _vBouncingDir, float _fDegree)
{
	// TODO : 랜덤 사이각 만들기
	m_vBouncingDir = _vBouncingDir;
}