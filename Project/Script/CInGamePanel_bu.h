#pragma once
#include <Engine\CScript.h>
class CInGamePanel_bu : public CScript
{
private:
	CGameObject*	m_pGunImageObj;
	CGameObject*	m_pMagazineImageObj;
	CGameObject*	m_pBulletImageObj;
	CGameObject*	m_pSkelletonImageObj;
private:
	CImageUI*		m_pGunImage;
	CImageUI*		m_pMagazineImage;
	CImageUI*		m_pBulletImage;
	CImageUI*		m_pSkelletonImage;

public:
	virtual void Awake() override;
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