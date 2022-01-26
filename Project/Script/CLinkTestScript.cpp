#include "pch.h"
#include "CLinkTestScript.h"
#include <Engine\CGameObject.h>
#include <Engine\CComponent.h>
#include <Engine\CScript.h>



CLinkTestScript::CLinkTestScript() :
	CScript((UINT)SCRIPT_TYPE::LINKTESTSCRIPT)
{
	AddParam(TScriptParam(_T("pTargetObj"), E_ScriptParam::GAMEOBJ, &pTargetObj));
}


CLinkTestScript::CLinkTestScript(const CLinkTestScript& _origin) :
	CScript((UINT)SCRIPT_TYPE::LINKTESTSCRIPT),
	pTargetObj{_origin.pTargetObj}
{
	AddParam_LINKOBJ(pTargetObj, _origin.pTargetObj, CLinkTestScript);
	AddParam(TScriptParam(_T("pTargetObj"), E_ScriptParam::GAMEOBJ, &pTargetObj));
}

bool CLinkTestScript::SaveToScene(FILE* _pFile)
{
	FWriteObj2(pTargetObj, _pFile);

	return true;
}

bool CLinkTestScript::LoadFromScene(FILE* _pFile)
{
	FReadObj2(&pTargetObj, this, _pFile);

	return true;
}