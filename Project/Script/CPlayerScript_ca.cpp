#include "pch.h"
#include "CPlayerScript_ca.h"

CPlayerScript_ca::CPlayerScript_ca() :
	CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT_CA),
	m_fSpeed(20.f),
	m_vTempVec{10.f,10.f,10.f},
	pTargetObj{nullptr}
	
{
	AddParam(TScriptParam{ _T("Speed"), E_ScriptParam::FLOAT, &m_fSpeed });
	AddParam(TScriptParam{ _T("TempVec"), E_ScriptParam::VEC3, &m_vTempVec });
	AddParam(TScriptParam{ _T("Object"), E_ScriptParam::GAMEOBJ, pTargetObj });
}

CPlayerScript_ca::~CPlayerScript_ca()
{
}

void CPlayerScript_ca::Start()
{
}

void CPlayerScript_ca::Update()
{
}