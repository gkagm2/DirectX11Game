#include "pch.h"
#include "CActivatorController.h"

CActivatorController::CActivatorController() :
	CScript((UINT)SCRIPT_TYPE::ACTIVATORCONTROLLER),
	m_pTargetActiveObj(nullptr)
{
	AddParam(TScriptParam{ _T("TargetObj"), E_ScriptParam::GAMEOBJ, (void**)&m_pTargetActiveObj });
}
void CActivatorController::Activate()
{
	if(m_pTargetActiveObj)
		m_pTargetActiveObj->SetActive(true,true);
}

void CActivatorController::Deactivate()
{
	if(m_pTargetActiveObj)
		m_pTargetActiveObj->SetActive(false, true);
}

bool CActivatorController::SaveToScene(FILE* _pFile)
{
	FWriteLinkObj(m_pTargetActiveObj, _pFile);
	return true;
}

bool CActivatorController::LoadFromScene(FILE* _pFile)
{
	FReadLinkObj((CObject**)&m_pTargetActiveObj, _pFile);

	return true;
}
