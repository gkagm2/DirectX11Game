#include "pch.h"
#include "CActivatorController.h"

CActivatorController::CActivatorController() :
	CScript((UINT)SCRIPT_TYPE::ACTIVATORCONTROLLER),
	m_pTargetActiveObj(nullptr)
{
	AddParam(TScriptParam{ _T("TargetObj"), E_ScriptParam::GAMEOBJ, &m_pTargetActiveObj });
	AddParam(TScriptParam{ _T("TargetObj2"), E_ScriptParam::GAMEOBJ, &m_pTargetActive2Obj });
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
	FWriteObj(m_pTargetActiveObj, _pFile);
	FWriteObj(m_pTargetActive2Obj, _pFile);
	return true;
}

bool CActivatorController::LoadFromScene(FILE* _pFile)
{
	FReadObj(&m_pTargetActiveObj, _pFile);
	FReadObj(&m_pTargetActive2Obj, _pFile);

	return true;
}
