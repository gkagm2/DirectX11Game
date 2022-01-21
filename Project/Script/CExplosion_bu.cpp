#include "pch.h"
#include "CExplosion_bu.h"

CExplosion_bu::CExplosion_bu() :
	CScript((UINT)SCRIPT_TYPE::EXPLOSION_BU),
	m_vExplosionDir{},
	m_fForce{ 10.f },
	m_fForceTime{ 0.f },
	m_fMaxForceTime{ 1.f },
	m_fDelTime(0.1f)
{
}

CExplosion_bu::CExplosion_bu(const CExplosion_bu& _origin):
	CScript((UINT)SCRIPT_TYPE::EXPLOSION_BU),
	m_vExplosionDir{_origin.m_vExplosionDir},
	m_fForce{ _origin.m_fForce},
	m_fForceTime{ 0.f},
	m_fMaxForceTime{ _origin.m_fMaxForceTime }
{
}

void CExplosion_bu::Awake()
{
	assert(Rigidbody2D());
	assert(Collider2D());
}

void CExplosion_bu::Update()
{
	/*if (m_fTouchTime > m_fDelTime) {
		m_fForceTime = m_fMaxForceTime;
	}*/

	m_fForceTime += DT;
	if (m_fForceTime < m_fMaxForceTime)
		Rigidbody2D()->AddForce(m_vExplosionDir * m_fForce);
}

void CExplosion_bu::SetExplosion(const Vector3& _vExplosionDir, float _fForce, float _fForceTime)
{
	m_vExplosionDir = _vExplosionDir;
	m_fForce = _fForce;
	m_fMaxForceTime = _fForceTime;
	m_fForceTime = 0.f;
}

void CExplosion_bu::OnCollisionStay2D(CCollider2D* _pCol)
{
	UINT iWallLayer = (UINT)E_Layer::Wall;
	if (iWallLayer == _pCol->GetGameObject()->GetLayer()) {
		m_fTouchTime += DT;
	}
}