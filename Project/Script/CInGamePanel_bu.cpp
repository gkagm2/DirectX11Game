#include "pch.h"
#include "CInGamePanel_bu.h"
#include "CPlayerController_bu.h"
#include "CWeapon_bu.h"

CInGamePanel_bu::CInGamePanel_bu() :
	CScript((UINT)SCRIPT_TYPE::INGAMEPANEL_BU),
	m_fUpdateTime(0.f),
	m_fMaxUpdateTime(0.1f),
	m_fGunNameMaxShowTime{ 1.4f }
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
	m_pHpImageObj = GetGameObject()->FindGameObjectInChilds(_T("HpImage"));
	m_pArmorImageObj = GetGameObject()->FindGameObjectInChilds(_T("ArmorImage"));
	m_pGunNameTextObj = GetGameObject()->FindGameObjectInChilds(_T("GunNameText"));

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
	m_pHpImage = m_pHpImageObj->GetComponent<CImageUI>();
	m_pArmorImage = m_pArmorImageObj->GetComponent<CImageUI>();
	m_pGunNameText = m_pGunNameTextObj->GetComponent<CTextUI>();

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
	assert(m_pHpImage);
	assert(m_pArmorImage);
	assert(m_pGunNameText);
	isCallAwake = true;
}

void CInGamePanel_bu::OnEnable()
{
	if (!isCallAwake)
		Awake();
	if (!isCallStart)
		Start();
}

void CInGamePanel_bu::Start()
{
	FIND_Component(m_pPlayer, CPlayerController_bu);
	assert(m_pPlayer);
	isCallStart = true;
	m_fGunNameShowTime = m_fGunNameShowTime;
}

void CInGamePanel_bu::Update()
{
	m_fUpdateTime += DT;
	m_fGunNameShowTime += DT;
	if (m_fUpdateTime > m_fMaxUpdateTime) {
		E_WeaponType_bu eWeaponType = m_pPlayer->GetWeapon()->GetCurWeaponType();
		m_pGunImage->Animator2D()->GetCurAnimation()->SetCurAnimationFrame((UINT)eWeaponType);

		float fHp = m_pPlayer->GetHp();
		if (fHp >= 80.f)
			m_pSkelletonImage->Animator2D()->Play(_T("Skelleton80HpUpSprite"));
		else if (fHp >= 60.f)
			m_pSkelletonImage->Animator2D()->Play(_T("Skelleton60HpUpSprite"));
		else if (fHp >= 20.f)
			m_pSkelletonImage->Animator2D()->Play(_T("Skelleton10HpUpSprite"));
		else
			m_pSkelletonImage->Animator2D()->Play(_T("Skelleton0HpSprite"));
			
		Vector3 vArmorImgPos = m_pArmorImage->RectTransform()->GetLocalPosition();
		Vector3 vHpImgPos = m_pHpImage->RectTransform()->GetLocalPosition();

		// 550이 완성이면
		const float fMaxHeightHp = 550.f, fMaxHeightArmor = 550.f;
		float fHpRatio = m_pPlayer->GetHp() / m_pPlayer->GetMaxHp();
		float fArmorRatio = m_pPlayer->GetArmor() / m_pPlayer->GetMaxArmor();
		m_pArmorImage->RectTransform()->SetHeight(fMaxHeightArmor * fArmorRatio);
		m_pHpImage->RectTransform()->SetHeight(fMaxHeightHp * fHpRatio);

		float vArmorImgHeight = m_pArmorImage->RectTransform()->GetHeight();
		float vHpImgHeight = m_pHpImage->RectTransform()->GetHeight();

		vHpImgPos.y = -((fMaxHeightHp - vHpImgHeight) * 0.5f);
		m_pHpImage->RectTransform()->SetLocalPosition(vHpImgPos);
		vArmorImgPos.y = -((fMaxHeightArmor - vArmorImgHeight) * 0.5f);
		m_pArmorImage->RectTransform()->SetLocalPosition(vArmorImgPos);

		m_pHpText->SetText(to_tstring((int)m_pPlayer->GetHp()));
		m_pArmorText->SetText(to_tstring((int)m_pPlayer->GetArmor()));
		TWeaponInfo_bu& tWeaponInfo = m_pPlayer->GetWeapon()->GetCurWeapon();
		m_pCurBulletText->SetText(to_tstring(tWeaponInfo.iCurBullet));
		TWeaponInfo_bu* pWeaponInfos = m_pPlayer->GetWeapon()->GetAllWeaponInfo();
		m_pShotgunBulletText->SetText(to_tstring(pWeaponInfos[(UINT)E_WeaponType_bu::Shotgun].iCurBullet));
		m_pMachineGunBulletText->SetText(to_tstring(pWeaponInfos[(UINT)E_WeaponType_bu::MachineGun].iCurBullet));
		m_pGrenadeLauncherBulletText->SetText(to_tstring(pWeaponInfos[(UINT)E_WeaponType_bu::GrenadeLauncher].iCurBullet));
		m_pFlameBulletText->SetText(to_tstring(pWeaponInfos[(UINT)E_WeaponType_bu::FlameThrower].iCurBullet));
		m_pLaserGunBulletText->SetText(to_tstring(pWeaponInfos[(UINT)E_WeaponType_bu::LaserGun].iCurBullet));

		if (m_fGunNameShowTime < m_fGunNameMaxShowTime)
			m_pGunNameText->SetText(m_strGunName); 
		else {
			if (m_pGunNameText->GetGameObject()->IsActive())
				m_pGunNameText->GetGameObject()->SetActive(false);
		}

		m_fUpdateTime = 0.f;
	}
}

void CInGamePanel_bu::ChangeGunName(const tstring& _strName)
{
	m_fGunNameShowTime = 0.f;
	m_strGunName = _strName;
	m_pGunNameText->GetGameObject()->SetActive(true);
}

bool CInGamePanel_bu::SaveToScene(FILE* _pFile)
{
	return true;
}

bool CInGamePanel_bu::LoadFromScene(FILE* _pFile)
{
	return true;
}
