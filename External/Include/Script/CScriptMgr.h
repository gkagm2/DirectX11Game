#pragma once

#include <vector>
#include <string>

enum class SCRIPT_TYPE
{
	BOOSTERSCRIPT_CA,
	BULLETSCRIPT_SH,
	CAMERAFOLLOWERSCRIPT,
	CHARACTERSCRIPT_CA,
	COMMANDMODULESCRIPT_CA,
	ENEMYRESPAWNERSCRIPT_SH,
	ENEMYSCRIPT_SH,
	GAMEMANAGERSCRIPT_SH,
	GIRDERSCRIPT_CA,
	KEYCONTROLLERSCRIPT_CA,
	LASERSCRIPT_CA,
	MODULESCRIPT_CA,
	MOUSECOLLISION2DSCRIPT,
	OBJECTDRAGANDDROPSCRIPT_CA,
	PLAYERSCRIPT_CA,
	PLAYERSCRIPT_SH,
	ROTATEZSCRIPT,
	TESTLIGHT2DSCRIPT,
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
