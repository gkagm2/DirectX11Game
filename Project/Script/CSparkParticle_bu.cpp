#include "pch.h"
#include "CSparkParticle_bu.h"

CSparkParticle_bu::CSparkParticle_bu() :
	CScript((UINT)SCRIPT_TYPE::SPARKPARTICLE_BU),
	m_fLifeTime(0.f),
	m_fMaxLifeTime(0.5f)
{
	AddParam(TScriptParam{ _T("Max Life Time"), E_ScriptParam::FLOAT, &m_fMaxLifeTime });
}

CSparkParticle_bu::CSparkParticle_bu(const CSparkParticle_bu& _origin) :
	CScript((UINT)SCRIPT_TYPE::SPARKPARTICLE_BU),
	m_fLifeTime(_origin.m_fLifeTime),
	m_fMaxLifeTime(_origin.m_fMaxLifeTime)
{
	AddParam(TScriptParam{ _T("Max Life Time"), E_ScriptParam::FLOAT, &m_fMaxLifeTime });
}

CSparkParticle_bu::~CSparkParticle_bu()
{
}

void CSparkParticle_bu::Awake()
{
	assert(ParticleSystem());
	m_pPS = ParticleSystem();
}

void CSparkParticle_bu::Update()
{
	m_fLifeTime += DT;
	if (m_fLifeTime >= m_fMaxLifeTime)
		DestroyGameObjectEvn(GetGameObject());
}

bool CSparkParticle_bu::SaveToScene(FILE* _pFile)
{
	FWrite(m_fMaxLifeTime, _pFile);
	return true;
}

bool CSparkParticle_bu::LoadFromScene(FILE* _pFile)
{
	FRead(m_fMaxLifeTime, _pFile);
	return true;
}
