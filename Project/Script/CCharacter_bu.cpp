#include "pch.h"
#include "CCharacter_bu.h"
#include "CWeapon_bu.h"

CCharacter_bu::CCharacter_bu() :
	CScript((UINT)SCRIPT_TYPE::CHARACTER_BU),
	m_fJumpPower{ 0.2f },
	m_fMovePower{ 0.2f },
	m_vCurMoveDir{},
	m_fHp{},
	m_fArmor{},
	m_pChainSawObj{ nullptr },
	m_pGunImageObj{ nullptr }

{
}

CCharacter_bu::CCharacter_bu(UINT _iScriptType) :
	CScript(_iScriptType),
	m_fJumpPower{ 0.2f },
	m_fMovePower{ 0.2f },
	m_vCurMoveDir{},
	m_fHp{},
	m_fArmor{},
	m_pChainSawObj{ nullptr },
	m_pGunImageObj{ nullptr }
{
}

CCharacter_bu::~CCharacter_bu()
{
}

void CCharacter_bu::Awake()
{

	m_pWeapon = GetGameObject()->GetComponent<CWeapon_bu>();	
	assert(m_pWeapon);
}

void CCharacter_bu::DamagedMe(float _fDamage)
{
	// armor가 존재하면
	float fDamage = 0.f;
	if (m_fArmor > 0.f) {
		m_fArmor = m_fArmor - _fDamage;
		if (m_fArmor < 0.f) {
			fDamage = fabsf(m_fArmor);
			m_fArmor = 0.f;
		}
	}
	m_fHp = m_fHp - fDamage;
	m_fHp = max(m_fHp, 0.f);
}

void CCharacter_bu::OnDead()
{
	// Animation start
}

void CCharacter_bu::ChangeWeapon(E_WeaponType_bu _eType)
{
	m_pWeapon->ChangeWeapon(_eType);
}
