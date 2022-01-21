#include "pch.h"
#include "CGameManager_bu.h"
#include "CUIManager_bu.h"
#include "CUIContainer_bu.h"
#include "CMainPanel_bu.h"
#include "CInGamePanel_bu.h"

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
	CUIManager_bu* pUIMgr = nullptr;
	FIND_Component(pUIMgr, CUIManager_bu);
	if (pUIMgr) {
		CInGamePanel_bu* pInGamePanel = pUIMgr->GetContainer()->GetInGamePanel();
		pInGamePanel->GetGameObject()->SetActive(true, true);
		GetGameObject()->SetActive(false, true);
	}
}

void CGameManager_bu::StartSuccessStopMode()
{
	CGameObject* pUIMgr = FIND_GameObject(_T("UIManager"));
	CUIManager_bu* pmgr =  pUIMgr->GetComponent<CUIManager_bu>();
	pmgr->GetContainer()->GetMainPanel()->GetGameObject()->SetActive(true, true);
	pmgr->GetContainer()->GetInGamePanel()->GetGameObject()->SetActive(false, true);
}

void CGameManager_bu::StartPauseMode()
{
}

void CGameManager_bu::StartFailStopMode()
{
	CGameObject* pUIMgr = FIND_GameObject(_T("UIManager"));
	CUIManager_bu* pmgr = pUIMgr->GetComponent<CUIManager_bu>();
	pmgr->GetContainer()->GetMainPanel()->GetGameObject()->SetActive(true, true);
	pmgr->GetContainer()->GetInGamePanel()->GetGameObject()->SetActive(false, true);
}