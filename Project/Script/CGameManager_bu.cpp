#include "pch.h"
#include "CGameManager_bu.h"
#include "CSoundManager_bu.h"
#include "CUIManager_bu.h"
#include "CUIContainer_bu.h"
#include "CMainPanel_bu.h"
#include "CInGamePanel_bu.h"

SINGLETON_SCRIPT_CPP(CGameManager_bu);

CGameManager_bu::CGameManager_bu() :
	CScript((UINT)SCRIPT_TYPE::GAMEMANAGER_BU),
	m_bIsInit{false},
	m_eGameMode{E_GameMode_bu::Pause}
{
}

CGameManager_bu::~CGameManager_bu() {

}

void CGameManager_bu::Start()
{
	CSoundManager_bu* m_pSoundMgr = nullptr;
	FIND_Component(m_pSoundMgr, CSoundManager_bu);
	if (m_pSoundMgr)
		m_pSoundMgr->SetBGSound(CSoundManager_bu::E_BGSoundType::Menu);
}

void CGameManager_bu::PrevUpdate()
{
	if (m_bIsInit)
		return;
	m_pStage1Obj = FIND_GameObject(_T("Stage1Objs"));
	m_pStage2Obj = FIND_GameObject(_T("Stage2Objs"));
	m_pStage3Obj = FIND_GameObject(_T("Stage3Objs"));

	m_pStage1ColObj = FIND_GameObject(_T("Stage1Col"));
	m_pStage2ColObj = FIND_GameObject(_T("Stage2Col"));
	m_pStage3ColObj = FIND_GameObject(_T("Stage3Col"));

	m_pStage1ColObj->SetActive(true);
	m_pStage2ColObj->SetActive(false);
	m_pStage3ColObj->SetActive(false);
	m_pStage1Obj->SetActive(true);
	m_pStage2Obj->SetActive(false);
	m_pStage3Obj->SetActive(false);

	m_bIsInit = true;
}

void CGameManager_bu::SetGameMode(E_GameMode_bu _eMode)
{
	m_eGameMode = _eMode;
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
		
		CMainPanel_bu* pMainPanel = pUIMgr->GetContainer()->GetMainPanel();
		pMainPanel->GetGameObject()->SetActive(false, true);
	}

	CSoundManager_bu* m_pSoundMgr = nullptr;
	FIND_Component(m_pSoundMgr, CSoundManager_bu);
	if (m_pSoundMgr)
		m_pSoundMgr->SetBGSound(CSoundManager_bu::E_BGSoundType::InGame);
}

void CGameManager_bu::StartSuccessStopMode()
{
	CGameObject* pUIMgr = FIND_GameObject(_T("UIManager"));
	CUIManager_bu* pmgr =  pUIMgr->GetComponent<CUIManager_bu>();
	pmgr->GetContainer()->GetMainPanel()->GetGameObject()->SetActive(true, true);
	pmgr->GetContainer()->GetInGamePanel()->GetGameObject()->SetActive(false, true);

	CSoundManager_bu* m_pSoundMgr = nullptr;
	FIND_Component(m_pSoundMgr, CSoundManager_bu);
	if (m_pSoundMgr)
		m_pSoundMgr->SetBGSound(CSoundManager_bu::E_BGSoundType::Menu);
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

	CSoundManager_bu* m_pSoundMgr = nullptr;
	FIND_Component(m_pSoundMgr, CSoundManager_bu);
	if (m_pSoundMgr)
		m_pSoundMgr->SetBGSound(CSoundManager_bu::E_BGSoundType::Menu);
}