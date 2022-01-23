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
	virtual void Awake() override;
	void SetBGSound(E_BGSoundType _eType);

public:
	CLONE(CSoundManager_bu);
	CSoundManager_bu();
	virtual ~CSoundManager_bu() override;
};