#include "pch.h"
#include "CInGamePanel_bu.h"
#include "CPlayerController_bu.h"
#include "CWeapon_bu.h"

CInGamePanel_bu::CInGamePanel_bu() :
	CScript((UINT)SCRIPT_TYPE::INGAMEPANEL_BU),
	m_pGunImageObj(nullptr),
	m_pMagazineImageObj(nullptr),
	m_pBulletImageObj(nullptr),
	m_pSkelletonImageObj(nullptr),
	m_pGunImage(nullptr),
	m_pMagazineImage(nullptr),
	m_pBulletImage(nullptr),
	m_pSkelletonImage(nullptr),
	m_pPlayer(nullptr)
{
}

CInGamePanel_bu::~CInGamePanel_bu()
{
}

void CInGamePanel_bu::Awake()
{
	m_pGunImageObj = GetGameObject()->FindGameObjectInChilds(_T("GunImage"));
	m_pMagazineImageObj = GetGameObject()->FindGameObjectInChilds(_T("MagazineImage")); // 삭제 예정
	m_pBulletImageObj = GetGameObject()->FindGameObjectInChilds(_T("30"));
	m_pSkelletonImageObj = GetGameObject()->FindGameObjectInChilds(_T("SkelletonImage"));
	m_pHpTextObj = GetGameObject()->FindGameObjectInChilds(_T("HPText"));
	m_pArmorTextObj = GetGameObject()->FindGameObjectInChilds(_T("ArmorText"));
	m_pCurBulletTextObj = GetGameObject()->FindGameObjectInChilds(_T("CurBulletText"));
	m_pShotgunBulletTextObj = GetGameObject()->FindGameObjectInChilds(_T("ShotgunBulletText"));
	m_pMachineGunBulletTextObj = GetGameObject()->FindGameObjectInChilds(_T("MachinegunBulletText"));
	m_pGrenadeLauncherBulletTextObj = GetGameObject()->FindGameObjectInChilds(_T("GrenadeLauncherBulletText"));
	m_pFlameBulletTextObj = GetGameObject()->FindGameObjectInChilds(_T("FlameBulletText"));
	m_pLaserGunBulletTextObj = GetGameObject()->FindGameObjectInChilds(_T("RailGunBulletText"));

	m_pGunImage = m_pGunImageObj->GetComponent<CImageUI>();
	m_pMagazineImage = m_pMagazineImageObj->GetComponent<CImageUI>();
	m_pBulletImage = m_pBulletImageObj->GetComponent<CImageUI>();
	m_pSkelletonImage = m_pSkelletonImageObj->GetComponent<CImageUI>();
	m_pHpText = m_pHpTextObj->GetComponent<CTextUI>();
	m_pArmorText = m_pArmorTextObj->GetComponent<CTextUI>();
	m_pCurBulletText = m_pCurBulletTextObj->GetComponent<CTextUI>();
	m_pShotgunBulletText = m_pShotgunBulletTextObj->GetComponent<CTextUI>();
	m_pMachineGunBulletText = m_pMachineGunBulletTextObj->GetComponent<CTextUI>();
	m_pGrenadeLauncherBulletText = m_pGrenadeLauncherBulletTextObj->GetComponent<CTextUI>();
	m_pFlameBulletText = m_pFlameBulletTextObj->GetComponent<CTextUI>();
	m_pLaserGunBulletText = m_pLaserGunBulletTextObj->GetComponent<CTextUI>();
	assert(m_pGunImage);
	assert(m_pMagazineImage);
	assert(m_pBulletImage);
	assert(m_pSkelletonImage);
	assert(m_pHpText					);
	assert(m_pArmorText					);
	assert(m_pCurBulletText)			;
	assert(m_pShotgunBulletText			);
	assert(m_pMachineGunBulletText		);
	assert(m_pGrenadeLauncherBulletText	);
	assert(m_pFlameBulletText			);
	assert(m_pLaserGunBulletText		);
}

void CInGamePanel_bu::Start()
{
	FIND_Component(m_pPlayer, CPlayerController_bu);
	assert(m_pPlayer);
}

void CInGamePanel_bu::Update()
{
	E_WeaponType_bu eWeaponType =  m_pPlayer->GetWeapon()->GetCurWeaponType();
	m_pGunImage->Animator2D()->GetCurAnimation()->SetCurAnimationFrame((UINT)eWeaponType);

	//m_pSkelletonImage->Animator2D()->GetCurAnimation()->
	// TODO :  HP 차이에 따라서 스켈레톤 이미지를 바꾸게 하기.



	m_pHpText->SetText(to_tstring(m_pPlayer->GetHp()));
	m_pArmorText->SetText(to_tstring(m_pPlayer->GetArmor()));
	TWeaponInfo_bu& tWeaponInfo = m_pPlayer->GetWeapon()->GetCurWeapon();
	m_pCurBulletText->SetText(to_tstring(tWeaponInfo.iCurBullet));
	TWeaponInfo_bu* pWeaponInfos = m_pPlayer->GetWeapon()->GetAllWeaponInfo();
	m_pShotgunBulletText->SetText(to_tstring(pWeaponInfos[(UINT)E_WeaponType_bu::Shotgun].iCurBullet));
	m_pMachineGunBulletText->SetText(to_tstring(pWeaponInfos[(UINT)E_WeaponType_bu::MachineGun].iCurBullet));
	m_pGrenadeLauncherBulletText->SetText(to_tstring(pWeaponInfos[(UINT)E_WeaponType_bu::GrenadeLauncher].iCurBullet));
	m_pFlameBulletText->SetText(to_tstring(pWeaponInfos[(UINT)E_WeaponType_bu::FlameThrower].iCurBullet));
	m_pLaserGunBulletText->SetText(to_tstring(pWeaponInfos[(UINT)E_WeaponType_bu::LaserGun].iCurBullet));
}

bool CInGamePanel_bu::SaveToScene(FILE* _pFile)
{
	return true;
}

bool CInGamePanel_bu::LoadFromScene(FILE* _pFile)
{
	return true;
}
