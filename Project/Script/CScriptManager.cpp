#include "pch.h"
#include "CScriptManager.h"

#include "CBoosterScript_ca.h"
#include "CBulletScript_sh.h"
#include "CCameraFollowerScript.h"
#include "CCharacterScript_ca.h"
#include "CColliderTestScript.h"
#include "CCommandModuleScript_ca.h"
#include "CEnemyRespawnerScript_sh.h"
#include "CEnemyScript_sh.h"
#include "CGameManagerScript_sh.h"
#include "CGameManager_ca.h"
#include "CGirder1x2Script_ca.h"
#include "CGirderScript_ca.h"
#include "CKeyControllerScript_ca.h"
#include "CLaserScript_ca.h"
#include "CLookAtScript.h"
#include "CManagerContainer_ca.h"
#include "CModuleCreator_ca.h"
#include "CModuleScript_ca.h"
#include "CObjectDragAndDropScript_ca.h"
#include "CPlayerScript_ca.h"
#include "CPlayerScript_sh.h"
#include "CRotateZScript.h"
#include "CScriptManager.h"
#include "CTestLight2DScript.h"

void CScriptManager::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CBoosterScript_ca");
	_vec.push_back(L"CBulletScript_sh");
	_vec.push_back(L"CCameraFollowerScript");
	_vec.push_back(L"CCharacterScript_ca");
	_vec.push_back(L"CColliderTestScript");
	_vec.push_back(L"CCommandModuleScript_ca");
	_vec.push_back(L"CEnemyRespawnerScript_sh");
	_vec.push_back(L"CEnemyScript_sh");
	_vec.push_back(L"CGameManagerScript_sh");
	_vec.push_back(L"CGameManager_ca");
	_vec.push_back(L"CGirder1x2Script_ca");
	_vec.push_back(L"CGirderScript_ca");
	_vec.push_back(L"CKeyControllerScript_ca");
	_vec.push_back(L"CLaserScript_ca");
	_vec.push_back(L"CLookAtScript");
	_vec.push_back(L"CManagerContainer_ca");
	_vec.push_back(L"CModuleCreator_ca");
	_vec.push_back(L"CModuleScript_ca");
	_vec.push_back(L"CObjectDragAndDropScript_ca");
	_vec.push_back(L"CPlayerScript_ca");
	_vec.push_back(L"CPlayerScript_sh");
	_vec.push_back(L"CRotateZScript");
	_vec.push_back(L"CScriptManager");
	_vec.push_back(L"CTestLight2DScript");
}

CScript * CScriptManager::GetScript(const wstring& _strScriptName)
{
	if (L"CBoosterScript_ca" == _strScriptName)
		return new CBoosterScript_ca;
	if (L"CBulletScript_sh" == _strScriptName)
		return new CBulletScript_sh;
	if (L"CCameraFollowerScript" == _strScriptName)
		return new CCameraFollowerScript;
	if (L"CCharacterScript_ca" == _strScriptName)
		return new CCharacterScript_ca;
	if (L"CColliderTestScript" == _strScriptName)
		return new CColliderTestScript;
	if (L"CCommandModuleScript_ca" == _strScriptName)
		return new CCommandModuleScript_ca;
	if (L"CEnemyRespawnerScript_sh" == _strScriptName)
		return new CEnemyRespawnerScript_sh;
	if (L"CEnemyScript_sh" == _strScriptName)
		return new CEnemyScript_sh;
	if (L"CGameManagerScript_sh" == _strScriptName)
		return new CGameManagerScript_sh;
	if (L"CGameManager_ca" == _strScriptName)
		return new CGameManager_ca;
	if (L"CGirder1x2Script_ca" == _strScriptName)
		return new CGirder1x2Script_ca;
	if (L"CGirderScript_ca" == _strScriptName)
		return new CGirderScript_ca;
	if (L"CKeyControllerScript_ca" == _strScriptName)
		return new CKeyControllerScript_ca;
	if (L"CLaserScript_ca" == _strScriptName)
		return new CLaserScript_ca;
	if (L"CLookAtScript" == _strScriptName)
		return new CLookAtScript;
	if (L"CManagerContainer_ca" == _strScriptName)
		return new CManagerContainer_ca;
	if (L"CModuleCreator_ca" == _strScriptName)
		return new CModuleCreator_ca;
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
	if (L"CScriptManager" == _strScriptName)
		return new CScriptManager;
	if (L"CTestLight2DScript" == _strScriptName)
		return new CTestLight2DScript;
	return nullptr;
}

CScript * CScriptManager::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)E_ScriptType::BOOSTERSCRIPT_CA:
		return new CBoosterScript_ca;
		break;
	case (UINT)E_ScriptType::BULLETSCRIPT_SH:
		return new CBulletScript_sh;
		break;
	case (UINT)E_ScriptType::CAMERAFOLLOWERSCRIPT:
		return new CCameraFollowerScript;
		break;
	case (UINT)E_ScriptType::CHARACTERSCRIPT_CA:
		return new CCharacterScript_ca;
		break;
	case (UINT)E_ScriptType::COLLIDERTESTSCRIPT:
		return new CColliderTestScript;
		break;
	case (UINT)E_ScriptType::COMMANDMODULESCRIPT_CA:
		return new CCommandModuleScript_ca;
		break;
	case (UINT)E_ScriptType::ENEMYRESPAWNERSCRIPT_SH:
		return new CEnemyRespawnerScript_sh;
		break;
	case (UINT)E_ScriptType::ENEMYSCRIPT_SH:
		return new CEnemyScript_sh;
		break;
	case (UINT)E_ScriptType::GAMEMANAGERSCRIPT_SH:
		return new CGameManagerScript_sh;
		break;
	case (UINT)E_ScriptType::GAMEMANAGER_CA:
		return new CGameManager_ca;
		break;
	case (UINT)E_ScriptType::GIRDER1X2SCRIPT_CA:
		return new CGirder1x2Script_ca;
		break;
	case (UINT)E_ScriptType::GIRDERSCRIPT_CA:
		return new CGirderScript_ca;
		break;
	case (UINT)E_ScriptType::KEYCONTROLLERSCRIPT_CA:
		return new CKeyControllerScript_ca;
		break;
	case (UINT)E_ScriptType::LASERSCRIPT_CA:
		return new CLaserScript_ca;
		break;
	case (UINT)E_ScriptType::LOOKATSCRIPT:
		return new CLookAtScript;
		break;
	case (UINT)E_ScriptType::MANAGERCONTAINER_CA:
		return new CManagerContainer_ca;
		break;
	case (UINT)E_ScriptType::MODULECREATOR_CA:
		return new CModuleCreator_ca;
		break;
	case (UINT)E_ScriptType::MODULESCRIPT_CA:
		return new CModuleScript_ca;
		break;
	case (UINT)E_ScriptType::OBJECTDRAGANDDROPSCRIPT_CA:
		return new CObjectDragAndDropScript_ca;
		break;
	case (UINT)E_ScriptType::PLAYERSCRIPT_CA:
		return new CPlayerScript_ca;
		break;
	case (UINT)E_ScriptType::PLAYERSCRIPT_SH:
		return new CPlayerScript_sh;
		break;
	case (UINT)E_ScriptType::ROTATEZSCRIPT:
		return new CRotateZScript;
		break;
	case (UINT)E_ScriptType::SCRIPTMANAGER:
		return new CScriptManager;
		break;
	case (UINT)E_ScriptType::TESTLIGHT2DSCRIPT:
		return new CTestLight2DScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptManager::GetScriptName(CScript * _pScript)
{
	switch ((E_ScriptType)_pScript->GetScriptType())
	{
	case E_ScriptType::BOOSTERSCRIPT_CA:
		return L"CBoosterScript_ca";
		break;

	case E_ScriptType::BULLETSCRIPT_SH:
		return L"CBulletScript_sh";
		break;

	case E_ScriptType::CAMERAFOLLOWERSCRIPT:
		return L"CCameraFollowerScript";
		break;

	case E_ScriptType::CHARACTERSCRIPT_CA:
		return L"CCharacterScript_ca";
		break;

	case E_ScriptType::COLLIDERTESTSCRIPT:
		return L"CColliderTestScript";
		break;

	case E_ScriptType::COMMANDMODULESCRIPT_CA:
		return L"CCommandModuleScript_ca";
		break;

	case E_ScriptType::ENEMYRESPAWNERSCRIPT_SH:
		return L"CEnemyRespawnerScript_sh";
		break;

	case E_ScriptType::ENEMYSCRIPT_SH:
		return L"CEnemyScript_sh";
		break;

	case E_ScriptType::GAMEMANAGERSCRIPT_SH:
		return L"CGameManagerScript_sh";
		break;

	case E_ScriptType::GAMEMANAGER_CA:
		return L"CGameManager_ca";
		break;

	case E_ScriptType::GIRDER1X2SCRIPT_CA:
		return L"CGirder1x2Script_ca";
		break;

	case E_ScriptType::GIRDERSCRIPT_CA:
		return L"CGirderScript_ca";
		break;

	case E_ScriptType::KEYCONTROLLERSCRIPT_CA:
		return L"CKeyControllerScript_ca";
		break;

	case E_ScriptType::LASERSCRIPT_CA:
		return L"CLaserScript_ca";
		break;

	case E_ScriptType::LOOKATSCRIPT:
		return L"CLookAtScript";
		break;

	case E_ScriptType::MANAGERCONTAINER_CA:
		return L"CManagerContainer_ca";
		break;

	case E_ScriptType::MODULECREATOR_CA:
		return L"CModuleCreator_ca";
		break;

	case E_ScriptType::MODULESCRIPT_CA:
		return L"CModuleScript_ca";
		break;

	case E_ScriptType::OBJECTDRAGANDDROPSCRIPT_CA:
		return L"CObjectDragAndDropScript_ca";
		break;

	case E_ScriptType::PLAYERSCRIPT_CA:
		return L"CPlayerScript_ca";
		break;

	case E_ScriptType::PLAYERSCRIPT_SH:
		return L"CPlayerScript_sh";
		break;

	case E_ScriptType::ROTATEZSCRIPT:
		return L"CRotateZScript";
		break;

	case E_ScriptType::SCRIPTMANAGER:
		return L"CScriptManager";
		break;

	case E_ScriptType::TESTLIGHT2DSCRIPT:
		return L"CTestLight2DScript";
		break;

	}
	return nullptr;
}