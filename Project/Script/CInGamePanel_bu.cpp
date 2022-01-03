#include "pch.h"
#include "CInGamePanel_bu.h"

CInGamePanel_bu::CInGamePanel_bu() :
	CScript((UINT)SCRIPT_TYPE::INGAMEPANEL_BU),
	m_pGunImageObj(nullptr),
	m_pMagazineImageObj(nullptr),
	m_pBulletImageObj(nullptr),
	m_pSkelletonImageObj(nullptr),

	m_pGunImage(nullptr),
	m_pMagazineImage(nullptr),
	m_pBulletImage(nullptr),
	m_pSkelletonImage(nullptr)
{
	AddParam(TScriptParam{ _T("Gun Img Obj"), E_ScriptParam::GAMEOBJ, (void**)&m_pGunImageObj });
	AddParam(TScriptParam{ _T("Magazine Img Obj"), E_ScriptParam::GAMEOBJ, (void**)&m_pMagazineImageObj});
	AddParam(TScriptParam{ _T("Bullet Img Obj"), E_ScriptParam::GAMEOBJ, (void**)&m_pBulletImageObj });
	AddParam(TScriptParam{ _T("Skelleton Img Obj"), E_ScriptParam::GAMEOBJ, (void**)&m_pSkelletonImageObj });
}

CInGamePanel_bu::~CInGamePanel_bu()
{
}

void CInGamePanel_bu::Awake()
{
	m_pGunImage = m_pGunImageObj->GetComponent<CImageUI>();
	assert(m_pGunImage);
	m_pMagazineImage = m_pMagazineImageObj->GetComponent<CImageUI>();
	assert(m_pMagazineImage);
	m_pBulletImage = m_pBulletImageObj->GetComponent<CImageUI>();
	assert(m_pBulletImage);
	m_pSkelletonImage = m_pSkelletonImageObj->GetComponent<CImageUI>();
	assert(m_pSkelletonImage);
}

void CInGamePanel_bu::Start()
{
}

void CInGamePanel_bu::Update()
{
}

bool CInGamePanel_bu::SaveToScene(FILE* _pFile)
{
	FWriteLinkObj(m_pGunImageObj, _pFile);
	FWriteLinkObj(m_pMagazineImageObj, _pFile);
	FWriteLinkObj(m_pBulletImageObj, _pFile);
	FWriteLinkObj(m_pSkelletonImageObj, _pFile);
	
	return true;
}

bool CInGamePanel_bu::LoadFromScene(FILE* _pFile)
{
	FReadLinkObj((CObject**)&m_pGunImageObj, _pFile);
	FReadLinkObj((CObject**)&m_pMagazineImageObj, _pFile);
	FReadLinkObj((CObject**)&m_pBulletImageObj, _pFile);
	FReadLinkObj((CObject**)&m_pSkelletonImageObj, _pFile);


	return true;
}
