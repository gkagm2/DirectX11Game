#include "pch.h"
#include "CWeapon_bu.h"

CWeapon_bu::CWeapon_bu() :
	CScript((UINT)SCRIPT_TYPE::WEAPON_BU),
	m_eWeaponType(E_WeaponType_bu::Chain)
{
}

CWeapon_bu::~CWeapon_bu()
{
}

void CWeapon_bu::Start()
{
}

void CWeapon_bu::Update()
{
}

bool CWeapon_bu::SaveToScene(FILE* _pFile)
{
	FWrite(m_eWeaponType, _pFile);
	return true;
}

bool CWeapon_bu::LoadFromScene(FILE* _pFile)
{
	FRead(m_eWeaponType, _pFile);
	return true;
}
