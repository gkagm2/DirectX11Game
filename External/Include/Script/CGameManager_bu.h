#pragma once
#include <Engine\CScript.h>

enum class E_GameMode_bu {
	Play,
	Pause,
	SuccessStop,
	FailStop,
};

class CGameManager_bu : public CScript
{
	SINGLETON_SCRIPT(CGameManager_bu);
public:
	void SetGameMode(E_GameMode_bu _eMode);

	void StartPlayMode();
	void StartSuccessStopMode();
	void StartPauseMode();
	void StartFailStopMode();

public:
	CLONE(CGameManager_bu);
};