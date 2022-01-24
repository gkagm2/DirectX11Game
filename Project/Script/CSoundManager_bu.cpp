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

	m_pMachinegun = CResourceManager::GetInstance()->LoadRes<CSound>(_T("sound\\Butcher\\Machinegun.wav"));
	m_pShotgun = CResourceManager::GetInstance()->LoadRes<CSound>(_T("sound\\Butcher\\Shotgun.wav"));
	m_pScream1 = CResourceManager::GetInstance()->LoadRes<CSound>(_T("sound\\Butcher\\VoiceScream1.wav"));
	m_pScream2 = CResourceManager::GetInstance()->LoadRes<CSound>(_T("sound\\Butcher\\VoiceScream2.wav"));
	m_pScream3 = CResourceManager::GetInstance()->LoadRes<CSound>(_T("sound\\Butcher\\VoiceScream3.wav"));
	m_pPickupItem= CResourceManager::GetInstance()->LoadRes<CSound>(_T("sound\\Butcher\\pickupItem.wav"));
	m_pSpotLightExplosion = CResourceManager::GetInstance()->LoadRes<CSound>(_T("sound\\Butcher\\SpotLightExplosion.wav"));
	m_pFootStep1 = CResourceManager::GetInstance()->LoadRes<CSound>(_T("sound\\Butcher\\footsep1.wav"));
	m_pFootStep2 = CResourceManager::GetInstance()->LoadRes<CSound>(_T("sound\\Butcher\\footsep2.wav"));
	m_pFootStep3 = CResourceManager::GetInstance()->LoadRes<CSound>(_T("sound\\Butcher\\footsep3.wav"));

	m_pWeaponReady = CResourceManager::GetInstance()->LoadRes<CSound>(_T("sound\\Butcher\\weaponReady.wav"));
	m_pChainsawReady = CResourceManager::GetInstance()->LoadRes<CSound>(_T("sound\\Butcher\\chainsawReady.wav"));
	m_pChainsawIdle = CResourceManager::GetInstance()->LoadRes<CSound>(_T("sound\\Butcher\\chainsawIdle.wav"));
	m_pChainsawAttackStart = CResourceManager::GetInstance()->LoadRes<CSound>(_T("sound\\Butcher\\chainsawAttackStart.wav"));
	m_pChainsawAttacking = CResourceManager::GetInstance()->LoadRes<CSound>(_T("sound\\Butcher\\chainsawAttacking.wav"));
	m_pChainsawStopping = CResourceManager::GetInstance()->LoadRes<CSound>(_T("sound\\Butcher\\footsep3.wav"));
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
