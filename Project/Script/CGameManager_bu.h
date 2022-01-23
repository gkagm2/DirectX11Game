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

private:
	bool m_bIsInit;
	CGameObject* m_pStage1Obj;
	CGameObject* m_pStage2Obj;
	CGameObject* m_pStage3Obj;
	CGameObject* m_pStage1ColObj;
	CGameObject* m_pStage2ColObj;
	CGameObject* m_pStage3ColObj;

public:
	virtual void Start() override;
	virtual void PrevUpdate() override;


	void SetGameMode(E_GameMode_bu _eMode);

	void StartPlayMode();
	void StartSuccessStopMode();
	void StartPauseMode();
	void StartFailStopMode();

public:
	CLONE(CGameManager_bu);
};