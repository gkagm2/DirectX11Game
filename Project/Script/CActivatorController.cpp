#include "pch.h"
#include "CActivatorController.h"

CActivatorController::CActivatorController() :
	CScript((UINT)SCRIPT_TYPE::ACTIVATORCONTROLLER),
	m_pTargetActiveObj(nullptr)
{
	AddParam(TScriptParam{ _T("TargetObj"), E_ScriptParam::GAMEOBJ, (void**)&m_pTargetActiveObj });
	AddParam(TScriptParam{ _T("TargetObj2"), E_ScriptParam::GAMEOBJ, (void**)&m_pTargetActive2Obj });
}
void CActivatorController::Activate()
{
	if(m_pTargetActiveObj)
		m_pTargetActiveObj->SetActive(true, false);
	if (m_pTargetActive2Obj)
		m_pTargetActive2Obj->SetActive(true, false);
}

void CActivatorController::Deactivate()
{
	if(m_pTargetActiveObj)
		m_pTargetActiveObj->SetActive(false, false);
	if (m_pTargetActive2Obj)
		m_pTargetActive2Obj->SetActive(false, false);
}

bool CActivatorController::SaveToScene(FILE* _pFile)
{
	FWriteLinkObj(m_pTargetActiveObj, _pFile);
	FWriteLinkObj(m_pTargetActive2Obj, _pFile);
	return true;
}

bool CActivatorController::LoadFromScene(FILE* _pFile)
{
	FReadLinkObj((CObject**)&m_pTargetActiveObj, _pFile);
	FReadLinkObj((CObject**)&m_pTargetActive2Obj, _pFile);

	return true;
}
