#include "pch.h"
#include "CWeapon_bu.h"

CWeapon_bu::CWeapon_bu() :
	CScript((UINT)SCRIPT_TYPE::WEAPON_BU),
	m_tWeaponInfo{},
	m_eCurType(E_WeaponType_bu::Chainsaw)
{
	_InitWeaponInfo();
}

CWeapon_bu::CWeapon_bu(const CWeapon_bu& _origin) :
	CScript((UINT)SCRIPT_TYPE::WEAPON_BU),
	m_tWeaponInfo{},
	m_eCurType(E_WeaponType_bu::Chainsaw)
{
	_InitWeaponInfo();
}

CWeapon_bu::~CWeapon_bu()
{
}

void CWeapon_bu::Start()
{
	m_pChainSawObj = GetGameObject()->FindGameObjectInChilds(BUTCHER_ObjName_Chainsaw);
	m_pGunImageObj = GetGameObject()->FindGameObjectInChilds(BUTCHER_ObjName_GunImage);
	assert(m_pChainSawObj);
	assert(m_pGunImageObj);
}

void CWeapon_bu::Update()
{
}

bool CWeapon_bu::SaveToScene(FILE* _pFile)
{
	FWrite(m_eCurType, _pFile);
	return true;
}

bool CWeapon_bu::LoadFromScene(FILE* _pFile)
{
	FRead(m_eCurType, _pFile);
	return true;
}

void CWeapon_bu::AddWeaponItem(E_WeaponType_bu _eType)
{
	TWeaponInfo_bu& tWeapon = m_tWeaponInfo[(UINT)_eType];
	int totalBullet = min(tWeapon.iMaxBullet, tWeapon.iCurBullet + tWeapon.iGetBulletCnt);
	tWeapon.iCurBullet = totalBullet;
}

void CWeapon_bu::ChangeWeapon(E_WeaponType_bu _eType)
{
	m_eCurType = _eType;
	if (_eType == E_WeaponType_bu::Chainsaw) {
		m_pGunImageObj->SetActive(false);
		m_pChainSawObj->SetActive(true);
	}
	else {
		m_pGunImageObj->SetActive(true);
		m_pChainSawObj->SetActive(false);
		m_pGunImageObj->Animator2D()->SetCurAnimationFrame((UINT)m_eCurType);
	}
}

void CWeapon_bu::Reload()
{
	TWeaponInfo_bu& tWeapon = m_tWeaponInfo[(UINT)m_eCurType];
	int iLeftBullet = tWeapon.iCurBullet;
	if (iLeftBullet <= tWeapon.iMaxMagazine)
		tWeapon.iCurMagazineBullet = iLeftBullet;
	else
		tWeapon.iCurMagazineBullet = tWeapon.iMaxMagazine;
}

void CWeapon_bu::_InitWeaponInfo()
{
	{
		TWeaponInfo_bu& tInfo = m_tWeaponInfo[(UINT)E_WeaponType_bu::Chainsaw];
		tInfo = {
			tInfo.iMaxBullet = 0,
			tInfo.iCurBullet = 0,
			tInfo.iMaxMagazine = 0,
			tInfo.iCurMagazineBullet = 0,
			tInfo.iGetBulletCnt = 0,
			tInfo.iInfinity = true
		};
	}
	
	{
		TWeaponInfo_bu& tInfo = m_tWeaponInfo[(UINT)E_WeaponType_bu::Shotgun];
		tInfo = {
			tInfo.iMaxBullet = 24,
			tInfo.iCurBullet = 0,
			tInfo.iMaxMagazine = 2,
			tInfo.iCurMagazineBullet = 0,
			tInfo.iGetBulletCnt = 8,
			tInfo.iInfinity = false
		};
	}
	{
		TWeaponInfo_bu& tInfo = m_tWeaponInfo[(UINT)E_WeaponType_bu::MachineGun];
		tInfo = {
			tInfo.iMaxBullet = 75,
			tInfo.iCurBullet = 0,
			tInfo.iMaxMagazine = 25,
			tInfo.iCurMagazineBullet = 0,
			tInfo.iGetBulletCnt = 25,
			tInfo.iInfinity = false
		};
	}
	{
		TWeaponInfo_bu& tInfo = m_tWeaponInfo[(UINT)E_WeaponType_bu::FlameThrower];
		tInfo = {
			tInfo.iMaxBullet = 90,
			tInfo.iCurBullet = 0,
			tInfo.iMaxMagazine = 30,
			tInfo.iCurMagazineBullet = 0,
			tInfo.iGetBulletCnt = 8,
			tInfo.iInfinity = false
		};
	}
	{
		TWeaponInfo_bu& tInfo = m_tWeaponInfo[(UINT)E_WeaponType_bu::GrenadeLauncher];
		tInfo = {
			tInfo.iMaxBullet = 20,
			tInfo.iCurBullet = 0,
			tInfo.iMaxMagazine = 5,
			tInfo.iCurMagazineBullet = 0,
			tInfo.iGetBulletCnt = 5,
			tInfo.iInfinity = false
		};
	}
	{
		TWeaponInfo_bu& tInfo = m_tWeaponInfo[(UINT)E_WeaponType_bu::LaserGun];
		tInfo = {
			tInfo.iMaxBullet = 15,
			tInfo.iCurBullet = 0,
			tInfo.iMaxMagazine = 5,
			tInfo.iCurMagazineBullet = 0,
			tInfo.iGetBulletCnt = 2,
			tInfo.iInfinity = false
		};
	}
}
