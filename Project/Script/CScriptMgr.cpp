#include "pch.h"
#include "CScriptMgr.h"

#include "CBulletScript_sh.h"
#include "CEnemyRespawnerScript_sh.h"
#include "CEnemyScript_sh.h"
#include "CGameManagerScript_sh.h"
#include "CPlayerScript_sh.h"
#include "CTestLight2DScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CBulletScript_sh");
	_vec.push_back(L"CEnemyRespawnerScript_sh");
	_vec.push_back(L"CEnemyScript_sh");
	_vec.push_back(L"CGameManagerScript_sh");
	_vec.push_back(L"CPlayerScript_sh");
	_vec.push_back(L"CTestLight2DScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CBulletScript_sh" == _strScriptName)
		return new CBulletScript_sh;
	if (L"CEnemyRespawnerScript_sh" == _strScriptName)
		return new CEnemyRespawnerScript_sh;
	if (L"CEnemyScript_sh" == _strScriptName)
		return new CEnemyScript_sh;
	if (L"CGameManagerScript_sh" == _strScriptName)
		return new CGameManagerScript_sh;
	if (L"CPlayerScript_sh" == _strScriptName)
		return new CPlayerScript_sh;
	if (L"CTestLight2DScript" == _strScriptName)
		return new CTestLight2DScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::BULLETSCRIPT_SH:
		return new CBulletScript_sh;
		break;
	case (UINT)SCRIPT_TYPE::ENEMYRESPAWNERSCRIPT_SH:
		return new CEnemyRespawnerScript_sh;
		break;
	case (UINT)SCRIPT_TYPE::ENEMYSCRIPT_SH:
		return new CEnemyScript_sh;
		break;
	case (UINT)SCRIPT_TYPE::GAMEMANAGERSCRIPT_SH:
		return new CGameManagerScript_sh;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT_SH:
		return new CPlayerScript_sh;
		break;
	case (UINT)SCRIPT_TYPE::TESTLIGHT2DSCRIPT:
		return new CTestLight2DScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::BULLETSCRIPT_SH:
		return L"CBulletScript_sh";
		break;

	case SCRIPT_TYPE::ENEMYRESPAWNERSCRIPT_SH:
		return L"CEnemyRespawnerScript_sh";
		break;

	case SCRIPT_TYPE::ENEMYSCRIPT_SH:
		return L"CEnemyScript_sh";
		break;

	case SCRIPT_TYPE::GAMEMANAGERSCRIPT_SH:
		return L"CGameManagerScript_sh";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT_SH:
		return L"CPlayerScript_sh";
		break;

	case SCRIPT_TYPE::TESTLIGHT2DSCRIPT:
		return L"CTestLight2DScript";
		break;

	}
	return nullptr;
}