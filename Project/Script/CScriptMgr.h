#pragma once

#include <vector>
#include <string>

enum class SCRIPT_TYPE
{
	BOOSTERSCRIPT_CA,
	BULLETSCRIPT_SH,
	BULLET_BU,
	CAMERAFOLLOWERSCRIPT,
	CHARACTERSCRIPT_CA,
	CHARACTER_BU,
	COLLIDERTESTSCRIPT,
	COMMANDMODULESCRIPT_CA,
	CURSORSCRIPT,
	CURSOR_BU,
	ENEMYCONTROLLER_BU,
	ENEMYRESPAWNERSCRIPT_SH,
	ENEMYSCRIPT_SH,
	GAMEMANAGERSCRIPT_SH,
	GAMEMANAGER_BU,
	GAMEMANAGER_CA,
	GIRDER1X2SCRIPT_CA,
	GIRDERSCRIPT_CA,
	GROUND_BU,
	INGAMEPANEL_BU,
	INTERACTIVEOBJ_BU,
	ITEM_BU,
	KEYCONTROLLERSCRIPT_CA,
	LASERSCRIPT_CA,
	LOOKAT2DSCRIPT,
	LOOKATSCRIPT,
	MAINPANEL_BU,
	MANAGERCONTAINER_CA,
	MODULECREATOR_CA,
	MODULESCRIPT_CA,
	OBJECTDRAGANDDROPSCRIPT_CA,
	PLAYERCONTROLLER_BU,
	PLAYERSCRIPT_CA,
	PLAYERSCRIPT_SH,
	POSTPROCESSINGSCRIPT,
	ROTATEZSCRIPT,
	SPARKPARTICLE_BU,
	SWITCH_BU,
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
