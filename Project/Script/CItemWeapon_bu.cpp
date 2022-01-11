#include "pch.h"
#include "CItemWeapon_bu.h"
#include "CWeapon_bu.h"
#include "CCharacter_bu.h"

tstring CItemWeapon_bu::strDescription = _T("0 Chainsaw\n1 Shotgun\n2 MachineGun\n3 FlameThrower\n4 GranadeLauncher\n5 LaserGun");

CItemWeapon_bu::CItemWeapon_bu() :
    CItem_bu((UINT)SCRIPT_TYPE::ITEMWEAPON_BU),
    m_eWeaponType(E_WeaponType_bu::Shotgun)
{
	AddParam(TScriptParam{ _T("Weapon type"), E_ScriptParam::INT, &m_eWeaponType });
	int iType = CMyMath::Clamp((int)m_eWeaponType, 0, (int)E_WeaponType_bu::End);
	m_eWeaponType = (E_WeaponType_bu)iType;

	AddParam(TScriptParam{ _T("Weapon Description"), E_ScriptParam::STRING_PRINT, &strDescription });
}

CItemWeapon_bu::CItemWeapon_bu(const CItemWeapon_bu& _origin) :
	CItem_bu((UINT)SCRIPT_TYPE::ITEMWEAPON_BU),
	m_eWeaponType(_origin.m_eWeaponType)
{
	AddParam(TScriptParam{ _T("Weapon type"), E_ScriptParam::INT, &m_eWeaponType });
	int iType = CMyMath::Clamp((int)m_eWeaponType, 0, (int)E_WeaponType_bu::End);
	m_eWeaponType = (E_WeaponType_bu)iType;

	AddParam(TScriptParam{ _T("Weapon Description"), E_ScriptParam::STRING_PRINT, &strDescription });
}

CItemWeapon_bu::~CItemWeapon_bu()
{
}

void CItemWeapon_bu::ItemInitWeapon()
{
	if (E_WeaponType_bu::Shotgun == m_eWeaponType) {
		m_pItemAnim->Play(BUTCHER_AnimName_ItemWPShotgun);
	}
	else if (E_WeaponType_bu::FlameThrower == m_eWeaponType) {
		m_pItemAnim->Play(BUTCHER_AnimName_ItemWPFlameThrower);
	}
	else if (E_WeaponType_bu::GrenadeLauncher == m_eWeaponType) {
		m_pItemAnim->Play(BUTCHER_AnimName_ItemWPGrenadeLauncher);
	}
	else if (E_WeaponType_bu::LaserGun == m_eWeaponType) {
		m_pItemAnim->Play(BUTCHER_AnimName_ItemWPLaserGun);
	}
	else if (E_WeaponType_bu::MachineGun == m_eWeaponType) {
		m_pItemAnim->Play(BUTCHER_AnimName_ItemWPMachineGun);
	}
	else if (E_WeaponType_bu::Chainsaw == m_eWeaponType) {
		m_pItemAnim->Play(BUTCHER_AnimName_ItemWPChainsaw);
	}
	else {
		assert(nullptr);
	}
	m_strItemName = WeaponTypeToStr_bu(m_eWeaponType);
}

void CItemWeapon_bu::ItemWeapon()
{
	// 아이템 잠금 해제
	CWeapon_bu* pWeapon = m_pTargetCharacter->GetGameObject()->GetComponent<CWeapon_bu>();
	if (!pWeapon) {
		assert(nullptr);
		return;
	}
	pWeapon->SetUseableWeapon(m_eWeaponType, true);
	m_pTargetCharacter->ChangeWeapon(m_eWeaponType);
}

void CItemWeapon_bu::Start()
{
	_SetItemCallBack(&CItemWeapon_bu::ItemInitWeapon, &CItemWeapon_bu::ItemWeapon, this);
	InitItem();
}

bool CItemWeapon_bu::SaveToScene(FILE* _pFile)
{
	FWrite(m_eWeaponType, _pFile);
	return true;
}

bool CItemWeapon_bu::LoadFromScene(FILE* _pFile)
{
	FRead(m_eWeaponType, _pFile);
	return true;
}

