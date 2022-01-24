#pragma once
#include <Engine\CScript.h>
class CSoundManager_bu : public CScript
{
public:
	enum class E_BGSoundType {
		Menu,
		InGame,
	};

private:
	SharedPtr<CSound> m_pBG;
	SharedPtr<CSound> m_pInGameBG;

public:
	SharedPtr<CSound> m_pShotgun;
	SharedPtr<CSound> m_pMachinegun;
	SharedPtr<CSound> m_pScream1;
	SharedPtr<CSound> m_pScream2;
	SharedPtr<CSound> m_pScream3;
	SharedPtr<CSound> m_pPickupItem;
	SharedPtr<CSound> m_pSpotLightExplosion;
	SharedPtr<CSound> m_pFootStep1;
	SharedPtr<CSound> m_pFootStep2;
	SharedPtr<CSound> m_pFootStep3;


public:
	virtual void Awake() override;
	void SetBGSound(E_BGSoundType _eType);

public:
	CLONE(CSoundManager_bu);
	CSoundManager_bu();
	virtual ~CSoundManager_bu() override;
};