#include "pch.h"
#include "CGameManager_bu.h"

SINGLETON_SCRIPT_CPP(CGameManager_bu);

CGameManager_bu::CGameManager_bu() :
	CScript((UINT)SCRIPT_TYPE::GAMEMANAGER_BU)
{
}

CGameManager_bu::~CGameManager_bu() {

}

void CGameManager_bu::SetGameMode(E_GameMode_bu _eMode)
{
	switch (_eMode) {
	case E_GameMode_bu::Play:
		StartPlayMode();
		break;
	case E_GameMode_bu::SuccessStop:
		StartSuccessStopMode();
		break;
	case E_GameMode_bu::FailStop:
		StartFailStopMode();
		break;
	case E_GameMode_bu::Pause:
		StartPauseMode();
		break;
	default:
		assert(nullptr);
		break;
	}
}

void CGameManager_bu::StartPlayMode()
{
}

void CGameManager_bu::StartSuccessStopMode()
{
}

void CGameManager_bu::StartPauseMode()
{
}

void CGameManager_bu::StartFailStopMode()
{
}