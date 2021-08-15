#include "pch.h"
#include "CScriptMgr.h"

#include "CBoosterScript_ca.h"
#include "CBulletScript_sh.h"
#include "CCharacterScript_ca.h"
#include "CCommandModuleScript_ca.h"
#include "CEnemyRespawnerScript_sh.h"
#include "CEnemyScript_sh.h"
#include "CGameManagerScript_sh.h"
#include "CGirderScript_ca.h"
#include "CLaserScript_ca.h"
#include "CModuleScript_ca.h"
#include "CObjectDragAndDropScript_ca.h"
#include "CPlayerScript_ca.h"
#include "CPlayerScript_sh.h"
#include "CRotateZScript.h"
#include "CTestLight2DScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CBoosterScript_ca");
	_vec.push_back(L"CBulletScript_sh");
	_vec.push_back(L"CCharacterScript_ca");
	_vec.push_back(L"CCommandModuleScript_ca");
	_vec.push_back(L"CEnemyRespawnerScript_sh");
	_vec.push_back(L"CEnemyScript_sh");
	_vec.push_back(L"CGameManagerScript_sh");
	_vec.push_back(L"CGirderScript_ca");
	_vec.push_back(L"CLaserScript_ca");
	_vec.push_back(L"CModuleScript_ca");
	_vec.push_back(L"CObjectDragAndDropScript_ca");
	_vec.push_back(L"CPlayerScript_ca");
	_vec.push_back(L"CPlayerScript_sh");
	_vec.push_back(L"CRotateZScript");
	_vec.push_back(L"CTestLight2DScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CBoosterScript_ca" == _strScriptName)
		return new CBoosterScript_ca;
	if (L"CBulletScript_sh" == _strScriptName)
		return new CBulletScript_sh;
	if (L"CCharacterScript_ca" == _strScriptName)
		return new CCharacterScript_ca;
	if (L"CCommandModuleScript_ca" == _strScriptName)
		return new CCommandModuleScript_ca;
	if (L"CEnemyRespawnerScript_sh" == _strScriptName)
		return new CEnemyRespawnerScript_sh;
	if (L"CEnemyScript_sh" == _strScriptName)
		return new CEnemyScript_sh;
	if (L"CGameManagerScript_sh" == _strScriptName)
		return new CGameManagerScript_sh;
	if (L"CGirderScript_ca" == _strScriptName)
		return new CGirderScript_ca;
	if (L"CLaserScript_ca" == _strScriptName)
		return new CLaserScript_ca;
	if (L"CModuleScript_ca" == _strScriptName)
		return new CModuleScript_ca;
	if (L"CObjectDragAndDropScript_ca" == _strScriptName)
		return new CObjectDragAndDropScript_ca;
	if (L"CPlayerScript_ca" == _strScriptName)
		return new CPlayerScript_ca;
	if (L"CPlayerScript_sh" == _strScriptName)
		return new CPlayerScript_sh;
	if (L"CRotateZScript" == _strScriptName)
		return new CRotateZScript;
	if (L"CTestLight2DScript" == _strScriptName)
		return new CTestLight2DScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::BOOSTERSCRIPT_CA:
		return new CBoosterScript_ca;
		break;
	case (UINT)SCRIPT_TYPE::BULLETSCRIPT_SH:
		return new CBulletScript_sh;
		break;
	case (UINT)SCRIPT_TYPE::CHARACTERSCRIPT_CA:
		return new CCharacterScript_ca;
		break;
	case (UINT)SCRIPT_TYPE::COMMANDMODULESCRIPT_CA:
		return new CCommandModuleScript_ca;
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
	case (UINT)SCRIPT_TYPE::GIRDERSCRIPT_CA:
		return new CGirderScript_ca;
		break;
	case (UINT)SCRIPT_TYPE::LASERSCRIPT_CA:
		return new CLaserScript_ca;
		break;
	case (UINT)SCRIPT_TYPE::MODULESCRIPT_CA:
		return new CModuleScript_ca;
		break;
	case (UINT)SCRIPT_TYPE::OBJECTDRAGANDDROPSCRIPT_CA:
		return new CObjectDragAndDropScript_ca;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT_CA:
		return new CPlayerScript_ca;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT_SH:
		return new CPlayerScript_sh;
		break;
	case (UINT)SCRIPT_TYPE::ROTATEZSCRIPT:
		return new CRotateZScript;
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
	case SCRIPT_TYPE::BOOSTERSCRIPT_CA:
		return L"CBoosterScript_ca";
		break;

	case SCRIPT_TYPE::BULLETSCRIPT_SH:
		return L"CBulletScript_sh";
		break;

	case SCRIPT_TYPE::CHARACTERSCRIPT_CA:
		return L"CCharacterScript_ca";
		break;

	case SCRIPT_TYPE::COMMANDMODULESCRIPT_CA:
		return L"CCommandModuleScript_ca";
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

	case SCRIPT_TYPE::GIRDERSCRIPT_CA:
		return L"CGirderScript_ca";
		break;

	case SCRIPT_TYPE::LASERSCRIPT_CA:
		return L"CLaserScript_ca";
		break;

	case SCRIPT_TYPE::MODULESCRIPT_CA:
		return L"CModuleScript_ca";
		break;

	case SCRIPT_TYPE::OBJECTDRAGANDDROPSCRIPT_CA:
		return L"CObjectDragAndDropScript_ca";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT_CA:
		return L"CPlayerScript_ca";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT_SH:
		return L"CPlayerScript_sh";
		break;

	case SCRIPT_TYPE::ROTATEZSCRIPT:
		return L"CRotateZScript";
		break;

	case SCRIPT_TYPE::TESTLIGHT2DSCRIPT:
		return L"CTestLight2DScript";
		break;

	}
	return nullptr;
}