#include "pch.h"
#include "CScriptMgr.h"

#include "CBoosterScript_ca.h"
#include "CBulletScript_sh.h"
#include "CBullet_bu.h"
#include "CCameraFollowerScript.h"
#include "CCharacterScript_ca.h"
#include "CCharacter_bu.h"
#include "CColliderTestScript.h"
#include "CCommandModuleScript_ca.h"
#include "CCursorScript.h"
#include "CCursor_bu.h"
#include "CDoor_bu.h"
#include "CElevator_bu.h"
#include "CEnemyController_bu.h"
#include "CEnemyRespawnerScript_sh.h"
#include "CEnemyScript_sh.h"
#include "CEventFuncTest.h"
#include "CGameManagerScript_sh.h"
#include "CGameManager_bu.h"
#include "CGameManager_ca.h"
#include "CGirder1x2Script_ca.h"
#include "CGirderScript_ca.h"
#include "CGround_bu.h"
#include "CInGamePanel_bu.h"
#include "CInteractiveObj_bu.h"
#include "CItemArmor_bu.h"
#include "CItemBullet_bu.h"
#include "CItemHp_bu.h"
#include "CItemWeapon_bu.h"
#include "CItem_bu.h"
#include "CKeyControllerScript_ca.h"
#include "CLaserScript_ca.h"
#include "CLookAt2DScript.h"
#include "CLookAtScript.h"
#include "CMainPanel_bu.h"
#include "CManagerContainer_ca.h"
#include "CModuleCreator_ca.h"
#include "CModuleScript_ca.h"
#include "CMouseLookAt_bu.h"
#include "CMuzzleParticle_bu.h"
#include "CObjectDragAndDropScript_ca.h"
#include "CPlayerController_bu.h"
#include "CPlayerScript_ca.h"
#include "CPlayerScript_sh.h"
#include "CPostProcessingScript.h"
#include "CRotateZScript.h"
#include "CSparkParticle_bu.h"
#include "CSwitch_bu.h"
#include "CTargetLookAt_bu.h"
#include "CTestLight2DScript.h"
#include "CUIContainer_bu.h"
#include "CUIManager_bu.h"
#include "CWayPoint_bu.h"
#include "CWeapon_bu.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CBoosterScript_ca");
	_vec.push_back(L"CBulletScript_sh");
	_vec.push_back(L"CBullet_bu");
	_vec.push_back(L"CCameraFollowerScript");
	_vec.push_back(L"CCharacterScript_ca");
	_vec.push_back(L"CCharacter_bu");
	_vec.push_back(L"CColliderTestScript");
	_vec.push_back(L"CCommandModuleScript_ca");
	_vec.push_back(L"CCursorScript");
	_vec.push_back(L"CCursor_bu");
	_vec.push_back(L"CDoor_bu");
	_vec.push_back(L"CElevator_bu");
	_vec.push_back(L"CEnemyController_bu");
	_vec.push_back(L"CEnemyRespawnerScript_sh");
	_vec.push_back(L"CEnemyScript_sh");
	_vec.push_back(L"CEventFuncTest");
	_vec.push_back(L"CGameManagerScript_sh");
	_vec.push_back(L"CGameManager_bu");
	_vec.push_back(L"CGameManager_ca");
	_vec.push_back(L"CGirder1x2Script_ca");
	_vec.push_back(L"CGirderScript_ca");
	_vec.push_back(L"CGround_bu");
	_vec.push_back(L"CInGamePanel_bu");
	_vec.push_back(L"CInteractiveObj_bu");
	_vec.push_back(L"CItemArmor_bu");
	_vec.push_back(L"CItemBullet_bu");
	_vec.push_back(L"CItemHp_bu");
	_vec.push_back(L"CItemWeapon_bu");
	_vec.push_back(L"CItem_bu");
	_vec.push_back(L"CKeyControllerScript_ca");
	_vec.push_back(L"CLaserScript_ca");
	_vec.push_back(L"CLookAt2DScript");
	_vec.push_back(L"CLookAtScript");
	_vec.push_back(L"CMainPanel_bu");
	_vec.push_back(L"CManagerContainer_ca");
	_vec.push_back(L"CModuleCreator_ca");
	_vec.push_back(L"CModuleScript_ca");
	_vec.push_back(L"CMouseLookAt_bu");
	_vec.push_back(L"CMuzzleParticle_bu");
	_vec.push_back(L"CObjectDragAndDropScript_ca");
	_vec.push_back(L"CPlayerController_bu");
	_vec.push_back(L"CPlayerScript_ca");
	_vec.push_back(L"CPlayerScript_sh");
	_vec.push_back(L"CPostProcessingScript");
	_vec.push_back(L"CRotateZScript");
	_vec.push_back(L"CSparkParticle_bu");
	_vec.push_back(L"CSwitch_bu");
	_vec.push_back(L"CTargetLookAt_bu");
	_vec.push_back(L"CTestLight2DScript");
	_vec.push_back(L"CUIContainer_bu");
	_vec.push_back(L"CUIManager_bu");
	_vec.push_back(L"CWayPoint_bu");
	_vec.push_back(L"CWeapon_bu");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CBoosterScript_ca" == _strScriptName)
		return new CBoosterScript_ca;
	if (L"CBulletScript_sh" == _strScriptName)
		return new CBulletScript_sh;
	if (L"CBullet_bu" == _strScriptName)
		return new CBullet_bu;
	if (L"CCameraFollowerScript" == _strScriptName)
		return new CCameraFollowerScript;
	if (L"CCharacterScript_ca" == _strScriptName)
		return new CCharacterScript_ca;
	if (L"CCharacter_bu" == _strScriptName)
		return new CCharacter_bu;
	if (L"CColliderTestScript" == _strScriptName)
		return new CColliderTestScript;
	if (L"CCommandModuleScript_ca" == _strScriptName)
		return new CCommandModuleScript_ca;
	if (L"CCursorScript" == _strScriptName)
		return new CCursorScript;
	if (L"CCursor_bu" == _strScriptName)
		return new CCursor_bu;
	if (L"CDoor_bu" == _strScriptName)
		return new CDoor_bu;
	if (L"CElevator_bu" == _strScriptName)
		return new CElevator_bu;
	if (L"CEnemyController_bu" == _strScriptName)
		return new CEnemyController_bu;
	if (L"CEnemyRespawnerScript_sh" == _strScriptName)
		return new CEnemyRespawnerScript_sh;
	if (L"CEnemyScript_sh" == _strScriptName)
		return new CEnemyScript_sh;
	if (L"CEventFuncTest" == _strScriptName)
		return new CEventFuncTest;
	if (L"CGameManagerScript_sh" == _strScriptName)
		return new CGameManagerScript_sh;
	if (L"CGameManager_bu" == _strScriptName)
		return new CGameManager_bu;
	if (L"CGameManager_ca" == _strScriptName)
		return new CGameManager_ca;
	if (L"CGirder1x2Script_ca" == _strScriptName)
		return new CGirder1x2Script_ca;
	if (L"CGirderScript_ca" == _strScriptName)
		return new CGirderScript_ca;
	if (L"CGround_bu" == _strScriptName)
		return new CGround_bu;
	if (L"CInGamePanel_bu" == _strScriptName)
		return new CInGamePanel_bu;
	if (L"CInteractiveObj_bu" == _strScriptName)
		return new CInteractiveObj_bu;
	if (L"CItemArmor_bu" == _strScriptName)
		return new CItemArmor_bu;
	if (L"CItemBullet_bu" == _strScriptName)
		return new CItemBullet_bu;
	if (L"CItemHp_bu" == _strScriptName)
		return new CItemHp_bu;
	if (L"CItemWeapon_bu" == _strScriptName)
		return new CItemWeapon_bu;
	if (L"CItem_bu" == _strScriptName)
		return new CItem_bu;
	if (L"CKeyControllerScript_ca" == _strScriptName)
		return new CKeyControllerScript_ca;
	if (L"CLaserScript_ca" == _strScriptName)
		return new CLaserScript_ca;
	if (L"CLookAt2DScript" == _strScriptName)
		return new CLookAt2DScript;
	if (L"CLookAtScript" == _strScriptName)
		return new CLookAtScript;
	if (L"CMainPanel_bu" == _strScriptName)
		return new CMainPanel_bu;
	if (L"CManagerContainer_ca" == _strScriptName)
		return new CManagerContainer_ca;
	if (L"CModuleCreator_ca" == _strScriptName)
		return new CModuleCreator_ca;
	if (L"CModuleScript_ca" == _strScriptName)
		return new CModuleScript_ca;
	if (L"CMouseLookAt_bu" == _strScriptName)
		return new CMouseLookAt_bu;
	if (L"CMuzzleParticle_bu" == _strScriptName)
		return new CMuzzleParticle_bu;
	if (L"CObjectDragAndDropScript_ca" == _strScriptName)
		return new CObjectDragAndDropScript_ca;
	if (L"CPlayerController_bu" == _strScriptName)
		return new CPlayerController_bu;
	if (L"CPlayerScript_ca" == _strScriptName)
		return new CPlayerScript_ca;
	if (L"CPlayerScript_sh" == _strScriptName)
		return new CPlayerScript_sh;
	if (L"CPostProcessingScript" == _strScriptName)
		return new CPostProcessingScript;
	if (L"CRotateZScript" == _strScriptName)
		return new CRotateZScript;
	if (L"CSparkParticle_bu" == _strScriptName)
		return new CSparkParticle_bu;
	if (L"CSwitch_bu" == _strScriptName)
		return new CSwitch_bu;
	if (L"CTargetLookAt_bu" == _strScriptName)
		return new CTargetLookAt_bu;
	if (L"CTestLight2DScript" == _strScriptName)
		return new CTestLight2DScript;
	if (L"CUIContainer_bu" == _strScriptName)
		return new CUIContainer_bu;
	if (L"CUIManager_bu" == _strScriptName)
		return new CUIManager_bu;
	if (L"CWayPoint_bu" == _strScriptName)
		return new CWayPoint_bu;
	if (L"CWeapon_bu" == _strScriptName)
		return new CWeapon_bu;
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
	case (UINT)SCRIPT_TYPE::BULLET_BU:
		return new CBullet_bu;
		break;
	case (UINT)SCRIPT_TYPE::CAMERAFOLLOWERSCRIPT:
		return new CCameraFollowerScript;
		break;
	case (UINT)SCRIPT_TYPE::CHARACTERSCRIPT_CA:
		return new CCharacterScript_ca;
		break;
	case (UINT)SCRIPT_TYPE::CHARACTER_BU:
		return new CCharacter_bu;
		break;
	case (UINT)SCRIPT_TYPE::COLLIDERTESTSCRIPT:
		return new CColliderTestScript;
		break;
	case (UINT)SCRIPT_TYPE::COMMANDMODULESCRIPT_CA:
		return new CCommandModuleScript_ca;
		break;
	case (UINT)SCRIPT_TYPE::CURSORSCRIPT:
		return new CCursorScript;
		break;
	case (UINT)SCRIPT_TYPE::CURSOR_BU:
		return new CCursor_bu;
		break;
	case (UINT)SCRIPT_TYPE::DOOR_BU:
		return new CDoor_bu;
		break;
	case (UINT)SCRIPT_TYPE::ELEVATOR_BU:
		return new CElevator_bu;
		break;
	case (UINT)SCRIPT_TYPE::ENEMYCONTROLLER_BU:
		return new CEnemyController_bu;
		break;
	case (UINT)SCRIPT_TYPE::ENEMYRESPAWNERSCRIPT_SH:
		return new CEnemyRespawnerScript_sh;
		break;
	case (UINT)SCRIPT_TYPE::ENEMYSCRIPT_SH:
		return new CEnemyScript_sh;
		break;
	case (UINT)SCRIPT_TYPE::EVENTFUNCTEST:
		return new CEventFuncTest;
		break;
	case (UINT)SCRIPT_TYPE::GAMEMANAGERSCRIPT_SH:
		return new CGameManagerScript_sh;
		break;
	case (UINT)SCRIPT_TYPE::GAMEMANAGER_BU:
		return new CGameManager_bu;
		break;
	case (UINT)SCRIPT_TYPE::GAMEMANAGER_CA:
		return new CGameManager_ca;
		break;
	case (UINT)SCRIPT_TYPE::GIRDER1X2SCRIPT_CA:
		return new CGirder1x2Script_ca;
		break;
	case (UINT)SCRIPT_TYPE::GIRDERSCRIPT_CA:
		return new CGirderScript_ca;
		break;
	case (UINT)SCRIPT_TYPE::GROUND_BU:
		return new CGround_bu;
		break;
	case (UINT)SCRIPT_TYPE::INGAMEPANEL_BU:
		return new CInGamePanel_bu;
		break;
	case (UINT)SCRIPT_TYPE::INTERACTIVEOBJ_BU:
		return new CInteractiveObj_bu;
		break;
	case (UINT)SCRIPT_TYPE::ITEMARMOR_BU:
		return new CItemArmor_bu;
		break;
	case (UINT)SCRIPT_TYPE::ITEMBULLET_BU:
		return new CItemBullet_bu;
		break;
	case (UINT)SCRIPT_TYPE::ITEMHP_BU:
		return new CItemHp_bu;
		break;
	case (UINT)SCRIPT_TYPE::ITEMWEAPON_BU:
		return new CItemWeapon_bu;
		break;
	case (UINT)SCRIPT_TYPE::ITEM_BU:
		return new CItem_bu;
		break;
	case (UINT)SCRIPT_TYPE::KEYCONTROLLERSCRIPT_CA:
		return new CKeyControllerScript_ca;
		break;
	case (UINT)SCRIPT_TYPE::LASERSCRIPT_CA:
		return new CLaserScript_ca;
		break;
	case (UINT)SCRIPT_TYPE::LOOKAT2DSCRIPT:
		return new CLookAt2DScript;
		break;
	case (UINT)SCRIPT_TYPE::LOOKATSCRIPT:
		return new CLookAtScript;
		break;
	case (UINT)SCRIPT_TYPE::MAINPANEL_BU:
		return new CMainPanel_bu;
		break;
	case (UINT)SCRIPT_TYPE::MANAGERCONTAINER_CA:
		return new CManagerContainer_ca;
		break;
	case (UINT)SCRIPT_TYPE::MODULECREATOR_CA:
		return new CModuleCreator_ca;
		break;
	case (UINT)SCRIPT_TYPE::MODULESCRIPT_CA:
		return new CModuleScript_ca;
		break;
	case (UINT)SCRIPT_TYPE::MOUSELOOKAT_BU:
		return new CMouseLookAt_bu;
		break;
	case (UINT)SCRIPT_TYPE::MUZZLEPARTICLE_BU:
		return new CMuzzleParticle_bu;
		break;
	case (UINT)SCRIPT_TYPE::OBJECTDRAGANDDROPSCRIPT_CA:
		return new CObjectDragAndDropScript_ca;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERCONTROLLER_BU:
		return new CPlayerController_bu;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT_CA:
		return new CPlayerScript_ca;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT_SH:
		return new CPlayerScript_sh;
		break;
	case (UINT)SCRIPT_TYPE::POSTPROCESSINGSCRIPT:
		return new CPostProcessingScript;
		break;
	case (UINT)SCRIPT_TYPE::ROTATEZSCRIPT:
		return new CRotateZScript;
		break;
	case (UINT)SCRIPT_TYPE::SPARKPARTICLE_BU:
		return new CSparkParticle_bu;
		break;
	case (UINT)SCRIPT_TYPE::SWITCH_BU:
		return new CSwitch_bu;
		break;
	case (UINT)SCRIPT_TYPE::TARGETLOOKAT_BU:
		return new CTargetLookAt_bu;
		break;
	case (UINT)SCRIPT_TYPE::TESTLIGHT2DSCRIPT:
		return new CTestLight2DScript;
		break;
	case (UINT)SCRIPT_TYPE::UICONTAINER_BU:
		return new CUIContainer_bu;
		break;
	case (UINT)SCRIPT_TYPE::UIMANAGER_BU:
		return new CUIManager_bu;
		break;
	case (UINT)SCRIPT_TYPE::WAYPOINT_BU:
		return new CWayPoint_bu;
		break;
	case (UINT)SCRIPT_TYPE::WEAPON_BU:
		return new CWeapon_bu;
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

	case SCRIPT_TYPE::BULLET_BU:
		return L"CBullet_bu";
		break;

	case SCRIPT_TYPE::CAMERAFOLLOWERSCRIPT:
		return L"CCameraFollowerScript";
		break;

	case SCRIPT_TYPE::CHARACTERSCRIPT_CA:
		return L"CCharacterScript_ca";
		break;

	case SCRIPT_TYPE::CHARACTER_BU:
		return L"CCharacter_bu";
		break;

	case SCRIPT_TYPE::COLLIDERTESTSCRIPT:
		return L"CColliderTestScript";
		break;

	case SCRIPT_TYPE::COMMANDMODULESCRIPT_CA:
		return L"CCommandModuleScript_ca";
		break;

	case SCRIPT_TYPE::CURSORSCRIPT:
		return L"CCursorScript";
		break;

	case SCRIPT_TYPE::CURSOR_BU:
		return L"CCursor_bu";
		break;

	case SCRIPT_TYPE::DOOR_BU:
		return L"CDoor_bu";
		break;

	case SCRIPT_TYPE::ELEVATOR_BU:
		return L"CElevator_bu";
		break;

	case SCRIPT_TYPE::ENEMYCONTROLLER_BU:
		return L"CEnemyController_bu";
		break;

	case SCRIPT_TYPE::ENEMYRESPAWNERSCRIPT_SH:
		return L"CEnemyRespawnerScript_sh";
		break;

	case SCRIPT_TYPE::ENEMYSCRIPT_SH:
		return L"CEnemyScript_sh";
		break;

	case SCRIPT_TYPE::EVENTFUNCTEST:
		return L"CEventFuncTest";
		break;

	case SCRIPT_TYPE::GAMEMANAGERSCRIPT_SH:
		return L"CGameManagerScript_sh";
		break;

	case SCRIPT_TYPE::GAMEMANAGER_BU:
		return L"CGameManager_bu";
		break;

	case SCRIPT_TYPE::GAMEMANAGER_CA:
		return L"CGameManager_ca";
		break;

	case SCRIPT_TYPE::GIRDER1X2SCRIPT_CA:
		return L"CGirder1x2Script_ca";
		break;

	case SCRIPT_TYPE::GIRDERSCRIPT_CA:
		return L"CGirderScript_ca";
		break;

	case SCRIPT_TYPE::GROUND_BU:
		return L"CGround_bu";
		break;

	case SCRIPT_TYPE::INGAMEPANEL_BU:
		return L"CInGamePanel_bu";
		break;

	case SCRIPT_TYPE::INTERACTIVEOBJ_BU:
		return L"CInteractiveObj_bu";
		break;

	case SCRIPT_TYPE::ITEMARMOR_BU:
		return L"CItemArmor_bu";
		break;

	case SCRIPT_TYPE::ITEMBULLET_BU:
		return L"CItemBullet_bu";
		break;

	case SCRIPT_TYPE::ITEMHP_BU:
		return L"CItemHp_bu";
		break;

	case SCRIPT_TYPE::ITEMWEAPON_BU:
		return L"CItemWeapon_bu";
		break;

	case SCRIPT_TYPE::ITEM_BU:
		return L"CItem_bu";
		break;

	case SCRIPT_TYPE::KEYCONTROLLERSCRIPT_CA:
		return L"CKeyControllerScript_ca";
		break;

	case SCRIPT_TYPE::LASERSCRIPT_CA:
		return L"CLaserScript_ca";
		break;

	case SCRIPT_TYPE::LOOKAT2DSCRIPT:
		return L"CLookAt2DScript";
		break;

	case SCRIPT_TYPE::LOOKATSCRIPT:
		return L"CLookAtScript";
		break;

	case SCRIPT_TYPE::MAINPANEL_BU:
		return L"CMainPanel_bu";
		break;

	case SCRIPT_TYPE::MANAGERCONTAINER_CA:
		return L"CManagerContainer_ca";
		break;

	case SCRIPT_TYPE::MODULECREATOR_CA:
		return L"CModuleCreator_ca";
		break;

	case SCRIPT_TYPE::MODULESCRIPT_CA:
		return L"CModuleScript_ca";
		break;

	case SCRIPT_TYPE::MOUSELOOKAT_BU:
		return L"CMouseLookAt_bu";
		break;

	case SCRIPT_TYPE::MUZZLEPARTICLE_BU:
		return L"CMuzzleParticle_bu";
		break;

	case SCRIPT_TYPE::OBJECTDRAGANDDROPSCRIPT_CA:
		return L"CObjectDragAndDropScript_ca";
		break;

	case SCRIPT_TYPE::PLAYERCONTROLLER_BU:
		return L"CPlayerController_bu";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT_CA:
		return L"CPlayerScript_ca";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT_SH:
		return L"CPlayerScript_sh";
		break;

	case SCRIPT_TYPE::POSTPROCESSINGSCRIPT:
		return L"CPostProcessingScript";
		break;

	case SCRIPT_TYPE::ROTATEZSCRIPT:
		return L"CRotateZScript";
		break;

	case SCRIPT_TYPE::SPARKPARTICLE_BU:
		return L"CSparkParticle_bu";
		break;

	case SCRIPT_TYPE::SWITCH_BU:
		return L"CSwitch_bu";
		break;

	case SCRIPT_TYPE::TARGETLOOKAT_BU:
		return L"CTargetLookAt_bu";
		break;

	case SCRIPT_TYPE::TESTLIGHT2DSCRIPT:
		return L"CTestLight2DScript";
		break;

	case SCRIPT_TYPE::UICONTAINER_BU:
		return L"CUIContainer_bu";
		break;

	case SCRIPT_TYPE::UIMANAGER_BU:
		return L"CUIManager_bu";
		break;

	case SCRIPT_TYPE::WAYPOINT_BU:
		return L"CWayPoint_bu";
		break;

	case SCRIPT_TYPE::WEAPON_BU:
		return L"CWeapon_bu";
		break;

	}
	return nullptr;
}