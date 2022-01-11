#include "pch.h"
#include "CWeapon_bu.h"

CWeapon_bu::CWeapon_bu() :
	CScript((UINT)SCRIPT_TYPE::WEAPON_BU),
	m_tWeaponInfo{},
	m_eCurType(E_WeaponType_bu::Chainsaw),
	m_pChainSawObj{ nullptr },
	m_pGunImageObj{ nullptr },
	m_fReloadDelayTime{0.3f},
	m_arrWeaponUse{}
{
	_InitWeaponInfo();
}

CWeapon_bu::CWeapon_bu(const CWeapon_bu& _origin) :
	CScript((UINT)SCRIPT_TYPE::WEAPON_BU),
	m_tWeaponInfo{},
	m_eCurType(E_WeaponType_bu::Chainsaw),
	m_pChainSawObj{ nullptr },
	m_pGunImageObj{ nullptr },
	m_fFireTime{_origin.m_fFireTime},
	m_fMaxFireTime{_origin.m_fMaxFireTime},
	m_fReloadDelayTime{_origin.m_fReloadDelayTime},
	m_arrWeaponUse{}
{
	for (int i = 0; i < (int)E_WeaponType_bu::End; ++i)
		m_arrWeaponUse[i] = _origin.m_arrWeaponUse[i];
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
	m_fFireTime += DT;
	if (m_fFireTime >= m_fMaxFireTime) {
		if (GetCurWeapon().bInfinity)
			m_bIsEnableFire = true;
		else {
			if (GetCurWeapon().iCurBullet > 0)
				m_bIsEnableFire = true;
			else
				m_bIsEnableFire = false;
		}
	}
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

void CWeapon_bu::Fire()
{
	if (IsEnableFire()) {
		if(!GetCurWeapon().bInfinity)
			--GetCurWeapon().iCurBullet;
		m_fFireTime = 0.f;
	}
}

void CWeapon_bu::AddWeaponBulletItem(E_WeaponType_bu _eType)
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
	m_fMaxFireTime = m_tWeaponInfo[(UINT)m_eCurType].fRpm;
	m_fFireTime = m_fMaxFireTime - m_fReloadDelayTime;
}

void CWeapon_bu::_InitWeaponInfo()
{
	{
		TWeaponInfo_bu& tInfo = m_tWeaponInfo[(UINT)E_WeaponType_bu::Chainsaw];
		tInfo = {
			tInfo.iMaxBullet = 0,
			tInfo.iCurBullet = 0,
			tInfo.iGetBulletCnt = 0,
			tInfo.fRpm = 0.2f / 60.f,
			tInfo.bInfinity = true
		};
	}
	
	{
		TWeaponInfo_bu& tInfo = m_tWeaponInfo[(UINT)E_WeaponType_bu::Shotgun];
		tInfo = {
			tInfo.iMaxBullet = 24,
			tInfo.iCurBullet = 0,
			tInfo.iGetBulletCnt = 8,
			tInfo.fRpm = 1.f / 60.f,
			tInfo.bInfinity = false
		};
	}
	{
		TWeaponInfo_bu& tInfo = m_tWeaponInfo[(UINT)E_WeaponType_bu::MachineGun];
		tInfo = {
			tInfo.iMaxBullet = 75,
			tInfo.iCurBullet = 0,
			tInfo.iGetBulletCnt = 25,
			tInfo.fRpm = 0.3f / 60.f,
			tInfo.bInfinity = false
		};
	}
	{
		TWeaponInfo_bu& tInfo = m_tWeaponInfo[(UINT)E_WeaponType_bu::FlameThrower];
		tInfo = {
			tInfo.iMaxBullet = 90,
			tInfo.iCurBullet = 0,
			tInfo.iGetBulletCnt = 8,
			tInfo.fRpm = 0.8f / 60.f,
			tInfo.bInfinity = false
		};
	}
	{
		TWeaponInfo_bu& tInfo = m_tWeaponInfo[(UINT)E_WeaponType_bu::GrenadeLauncher];
		tInfo = {
			tInfo.iMaxBullet = 20,
			tInfo.iCurBullet = 0,
			tInfo.iGetBulletCnt = 5,
			tInfo.fRpm = 2.f / 60.f,
			tInfo.bInfinity = false
		};
	}
	{
		TWeaponInfo_bu& tInfo = m_tWeaponInfo[(UINT)E_WeaponType_bu::LaserGun];
		tInfo = {
			tInfo.iMaxBullet = 15,
			tInfo.iCurBullet = 0,
			tInfo.iGetBulletCnt = 2,
			tInfo.fRpm = 2.f / 60.f,
			tInfo.bInfinity = false
		};
	}
}
