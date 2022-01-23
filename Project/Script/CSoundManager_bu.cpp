#include "pch.h"
#include "CSoundManager_bu.h"
CSoundManager_bu::CSoundManager_bu() :
	CScript((UINT)SCRIPT_TYPE::SOUNDMANAGER_BU)
{
}

CSoundManager_bu::~CSoundManager_bu()
{
	if(m_pBG.Get())
		m_pBG->Stop();
	if(m_pInGameBG.Get())
		m_pInGameBG->Stop();
}

void CSoundManager_bu::Awake()
{
	// Sound Test
	/*SharedPtr<CSound> pSound = CResourceManager ::GetInstance()->LoadRes<CSound>(L"Sound", L"sound\\BGM_Stage1.wav");
	int iChannel = pSound->Play(0);
	pSound->SetVolume(0.6f, iChannel);*/
	m_pBG = CResourceManager::GetInstance()->LoadRes<CSound>(_T("sound\\Butcher\\001_menu.mp3"));
	m_pInGameBG = CResourceManager::GetInstance()->LoadRes<CSound>(_T("sound\\Butcher\\002_intro.mp3"));
}

void CSoundManager_bu::SetBGSound(E_BGSoundType _eType)
{
	switch(_eType) {
	case E_BGSoundType::Menu:
		m_pBG->Play(0);
		m_pInGameBG->Stop();
		break;
	case E_BGSoundType::InGame:
		m_pInGameBG->Play(0);
		m_pBG->Stop();
		break;
	default:
		assert(nullptr);
		break;
	}
}
