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
	if (pTargetObj) {
		tstring pName = pTargetObj->GetName();
	}
}

void CPlayerScript_ca::Update()
{
}

bool CPlayerScript_ca::SaveToScene(FILE* _pFile)
{
	FWrite(m_fSpeed, _pFile);
	FWrite(m_vTempVec, _pFile);

	UINT iLayer = pTargetObj->GetLayer();
	FWrite(iLayer, _pFile);
	pTargetObj->SaveToScene(_pFile);
	return true;
}

bool CPlayerScript_ca::LoadFromScene(FILE* _pFile)
{
	FRead(m_fSpeed, _pFile);
	FRead(m_vTempVec, _pFile);

	UINT iLayer = 0;
	FRead(iLayer, _pFile);
	pTargetObj->LoadFromScene(_pFile,iLayer);

	return true;
}
