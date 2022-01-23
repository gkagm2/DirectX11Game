#pragma once
#include <Engine\CScript.h>
class CInGamePanel_bu : public CScript
{
private:
	float m_fUpdateTime;
	float m_fMaxUpdateTime;
	bool isCallAwake;
	bool isCallStart;
private:
	CGameObject*	m_pGunImageObj;
	CGameObject*	m_pMagazineImageObj;
	CGameObject*	m_pBulletImageObj;
	CGameObject*	m_pSkelletonImageObj;
	CGameObject*	m_pHpTextObj;
	CGameObject*	m_pArmorTextObj;
	CGameObject*	m_pCurBulletTextObj;
	CGameObject*	m_pShotgunBulletTextObj;
	CGameObject*	m_pMachineGunBulletTextObj;
	CGameObject*	m_pGrenadeLauncherBulletTextObj;
	CGameObject*	m_pFlameBulletTextObj;
	CGameObject*	m_pLaserGunBulletTextObj;
	CGameObject* m_pHpImageObj;
	CGameObject* m_pArmorImageObj;

private:
	CImageUI*		m_pGunImage;
	CImageUI*		m_pMagazineImage;
	CImageUI*		m_pBulletImage;
	CImageUI*		m_pSkelletonImage;
	CTextUI*		m_pHpText;
	CTextUI*		m_pArmorText;
	CTextUI*		m_pCurBulletText;
	CTextUI*		m_pShotgunBulletText;
	CTextUI*		m_pMachineGunBulletText;
	CTextUI*		m_pGrenadeLauncherBulletText;
	CTextUI*		m_pFlameBulletText;
	CTextUI*		m_pLaserGunBulletText;
	CImageUI*		m_pHpImage;
	CImageUI*		m_pArmorImage;

private:
	class CPlayerController_bu* m_pPlayer;

public:
	virtual void Awake() override;
	virtual void OnEnable() override;
	virtual void Start() override;
	virtual void Update() override;

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CInGamePanel_bu);
	CInGamePanel_bu();
	virtual ~CInGamePanel_bu() override;
};