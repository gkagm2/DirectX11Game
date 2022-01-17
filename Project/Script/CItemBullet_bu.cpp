#include "pch.h"
#include "CItemBullet_bu.h"
#include "CWeapon_bu.h"
#include "CCharacter_bu.h"

tstring CItemBullet_bu::strDescription = _T("1 Shotgun\n2 MachineGun\n3 FlameThrower\n4 GrenadeLauncher\n5 LaserGun");

CItemBullet_bu::CItemBullet_bu() : 
	CItem_bu((UINT)SCRIPT_TYPE::ITEMBULLET_BU),
	m_eWeaponType(E_WeaponType_bu::Shotgun)
{
	AddParam(TScriptParam{ _T("Bullet type"), E_ScriptParam::INT, &m_eWeaponType });
	int iType = CMyMath::Clamp((int)m_eWeaponType, 0, (int)E_WeaponType_bu::End);
	m_eWeaponType = (E_WeaponType_bu)iType;

	AddParam(TScriptParam{ _T("Weapon Description"), E_ScriptParam::STRING_PRINT, &strDescription });
}

CItemBullet_bu::CItemBullet_bu(const CItemBullet_bu& _origin) :
	CItem_bu((UINT)SCRIPT_TYPE::ITEMBULLET_BU),
	m_eWeaponType(_origin.m_eWeaponType)
{
	AddParam(TScriptParam{ _T("Bullet type"), E_ScriptParam::INT, &m_eWeaponType });
	int iType = CMyMath::Clamp((int)m_eWeaponType, 0, (int)E_WeaponType_bu::End);
	m_eWeaponType = (E_WeaponType_bu)iType;

	AddParam(TScriptParam{ _T("Weapon Description"), E_ScriptParam::STRING_PRINT, &strDescription });
}

CItemBullet_bu::~CItemBullet_bu()
{
}

void CItemBullet_bu::InitItem()
{
	m_pItemAnimObj = GetGameObject()->FindGameObjectInChilds(BUTCHER_ObjName_ItemAnim);
	assert(m_pItemAnimObj);
	m_pItemAnim = m_pItemAnimObj->Animator2D();
	assert(m_pItemAnim);

	if (E_WeaponType_bu::Shotgun == m_eWeaponType) {
		m_pItemAnim->Play(BUTCHER_AnimName_ItemBulShotgun);
	}
	else if (E_WeaponType_bu::FlameThrower == m_eWeaponType) {
		m_pItemAnim->Play(BUTCHER_AnimName_ItemBulFlameThrower);
	}
	else if (E_WeaponType_bu::GrenadeLauncher == m_eWeaponType) {
		m_pItemAnim->Play(BUTCHER_AnimName_ItemBulGrenadeLauncher);
	}
	else if (E_WeaponType_bu::LaserGun == m_eWeaponType) {
		m_pItemAnim->Play(BUTCHER_AnimName_ItemBulLaserGun);
	}
	else if (E_WeaponType_bu::MachineGun == m_eWeaponType) {
		m_pItemAnim->Play(BUTCHER_AnimName_ItemBulMachineGun);
	}
	else {
		assert(nullptr);
	}
}

void CItemBullet_bu::Interact(CCharacter_bu* _pTargetCharacter)
{
	m_pTargetCharacter = _pTargetCharacter;
	// ¾ÆÀÌÅÛ ÃÑ¾Ë ´õÇØÁü
	CWeapon_bu* pWeapon = m_pTargetCharacter->GetGameObject()->GetComponent<CWeapon_bu>();
	if (!pWeapon) {
		assert(nullptr);
		return;
	}
	pWeapon->AddWeaponBulletItem(m_eWeaponType);
}

void CItemBullet_bu::Start()
{
	InitItem();
}

bool CItemBullet_bu::SaveToScene(FILE* _pFile)
{
	FWrite(m_eWeaponType, _pFile);
	return true;
}

bool CItemBullet_bu::LoadFromScene(FILE* _pFile)
{
	FRead(m_eWeaponType, _pFile);
	return true;
}