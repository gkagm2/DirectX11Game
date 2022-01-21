#include "pch.h"
#include "CExplosion_bu.h"

CExplosion_bu::CExplosion_bu() :
	CScript((UINT)SCRIPT_TYPE::EXPLOSION_BU),
	m_vExplosionDir{},
	m_fForce{ 10.f },
	m_fForceTime{ 0.f },
	m_fMaxForceTime{ 1.f }
{
	// ExplosionDir은 read용으로만 사용
	AddParam(TScriptParam{ _T("m_vExplosionDir"), E_ScriptParam::VEC3, &m_vExplosionDir });
	AddParam(TScriptParam{ _T("m_fForce"), E_ScriptParam::FLOAT, &m_fForce });
	AddParam(TScriptParam{ _T("m_fMaxForceTime"), E_ScriptParam::FLOAT, &m_fMaxForceTime});
}

CExplosion_bu::CExplosion_bu(const CExplosion_bu& _origin):
	CScript((UINT)SCRIPT_TYPE::EXPLOSION_BU),
	m_vExplosionDir{_origin.m_vExplosionDir},
	m_fForce{ _origin.m_fForce},
	m_fForceTime{ 0.f},
	m_fMaxForceTime{ _origin.m_fMaxForceTime }
{
	AddParam(TScriptParam{ _T("m_vExplosionDir"), E_ScriptParam::VEC3, &m_vExplosionDir });
	AddParam(TScriptParam{ _T("m_fForce"), E_ScriptParam::FLOAT, &m_fForce });
	AddParam(TScriptParam{ _T("m_fMaxForceTime"), E_ScriptParam::FLOAT, &m_fMaxForceTime });
}

void CExplosion_bu::Awake()
{
	assert(Rigidbody2D());
	assert(Collider2D());
}

void CExplosion_bu::Update()
{
	m_fForceTime += DT;
	if (m_fForceTime < m_fMaxForceTime)
		Rigidbody2D()->AddForce(m_vExplosionDir * m_fForce);
}

bool CExplosion_bu::SaveToScene(FILE* _pFile)
{
	FWrite(m_fForce, _pFile);
	FWrite(m_fMaxForceTime, _pFile);
	return true;
}

bool CExplosion_bu::LoadFromScene(FILE* _pFile)
{
	FRead(m_fForce, _pFile);
	FRead(m_fMaxForceTime, _pFile);
	return true;
}

void CExplosion_bu::SetExplosion(const Vector3& _vExplosionDir, float _fForce, float _fForceTime)
{
	m_vExplosionDir = _vExplosionDir;
	m_fForce = _fForce;
	m_fMaxForceTime = m_fForceTime;
}