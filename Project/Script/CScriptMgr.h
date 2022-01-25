#pragma once

#include <vector>
#include <string>

enum class SCRIPT_TYPE
{
	ACTIVATORCONTROLLER,
	ACTIVATOR_BU,
	BOOSTERSCRIPT_CA,
	BULLETBOUNCINGPARTICLE_BU,
	BULLETSCRIPT_SH,
	BULLET_BU,
	CAMERAFOLLOWERSCRIPT,
	CHAINSAW_BU,
	CHARACTERSCRIPT_CA,
	CHARACTER_BU,
	COLLIDERTESTSCRIPT,
	COMMANDMODULESCRIPT_CA,
	CURSORSCRIPT,
	CURSOR_BU,
	DEACTIVATOR_BU,
	DOOR_BU,
	ELEVATOR_BU,
	ENEMYCONTROLLER_BU,
	ENEMYRESPAWNERSCRIPT_SH,
	ENEMYSCRIPT_SH,
	EVENTFUNCTEST,
	EXITDOORENDCOL_BU,
	EXITDOOR_BU,
	EXPLOSION_BU,
	FLAMEPARTICLE_BU,
	GAMEMANAGERSCRIPT_SH,
	GAMEMANAGER_BU,
	GAMEMANAGER_CA,
	GIRDER1X2SCRIPT_CA,
	GIRDERSCRIPT_CA,
	GROUNDCHECK_BU,
	GROUND_BU,
	INGAMEPANEL_BU,
	INTERACTIVEOBJ_BU,
	ITEMARMOR_BU,
	ITEMBULLET_BU,
	ITEMHP_BU,
	ITEMWEAPON_BU,
	ITEM_BU,
	KEYCONTROLLERSCRIPT_CA,
	LASERSCRIPT_CA,
	LINERENDERINGTESTSCRIPT,
	LINKTESTSCRIPT,
	LOOKAT2DSCRIPT,
	LOOKATSCRIPT,
	MAINPANEL_BU,
	MANAGERCONTAINER_CA,
	MODULECREATOR_CA,
	MODULESCRIPT_CA,
	MOUSELOOKAT_BU,
	MUZZLEPARTICLE_BU,
	OBJECTDRAGANDDROPSCRIPT_CA,
	OBJECTMANAGER_BU,
	PLAYERCONTROLLER_BU,
	PLAYERSCRIPT_CA,
	PLAYERSCRIPT_SH,
	PORTAL_BU,
	POSTPROCESSINGSCRIPT,
	RESPAWNMANAGER_BU,
	RIGIDBODY2DTESTSCRIPT,
	ROTATEZSCRIPT,
	SOUNDMANAGER_BU,
	SPARKPARTICLE_BU,
	SPOTLIGHT_BU,
	SWITCH_BU,
	TARGETLOOKAT_BU,
	TESTLIGHT2DSCRIPT,
	UICONTAINER_BU,
	UIMANAGER_BU,
	WAYPOINT_BU,
	WEAPON_BU,
	END,
};

using namespace std;

class CScript;

class CScriptMgr
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static CScript * GetScript(const wstring& _strScriptName);
	static CScript * GetScript(UINT _iScriptType);
	static const wchar_t * GetScriptName(CScript * _pScript);
};
