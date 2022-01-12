#include "pch.h"
#include "CCharacter_bu.h"
#include "CWeapon_bu.h"

CCharacter_bu::CCharacter_bu() :
	CScript((UINT)SCRIPT_TYPE::CHARACTER_BU),
	m_fJumpPower{ 0.2f },
	m_fMovePower{ 0.2f },
	m_vCurMoveDir{},
	m_fHp{100.f},
	m_fMaxHp{ 100.f },
	m_fArmor{100.f},
	m_fMaxArmor{ 100.f },
	m_pGunRotationPosObj{ nullptr },
	m_pChainSawObj{ nullptr },
	m_pGunImageObj{ nullptr },
	m_pFlipObj{ nullptr },
	m_pFlipGunObj{ nullptr },
	m_pMuzzleObj{ nullptr },
	m_fShotTime(0.3f),
	m_fMaxShotTime(0.3f)

{
}

CCharacter_bu::CCharacter_bu(UINT _iScriptType) :
	CScript(_iScriptType),
	m_fJumpPower{ 0.2f },
	m_fMovePower{ 0.2f },
	m_vCurMoveDir{},
	m_fHp{100.f},
	m_fMaxHp{ 100.f },
	m_fArmor{100.f},
	m_fMaxArmor{ 100.f },
	m_pGunRotationPosObj{ nullptr },
	m_pChainSawObj{ nullptr },
	m_pGunImageObj{ nullptr },
	m_pMuzzleObj{ nullptr },
	m_pFlipObj{nullptr},
	m_pFlipGunObj{ nullptr }
{
}

CCharacter_bu::~CCharacter_bu()
{
}

void CCharacter_bu::Awake()
{
	m_pWeapon = GetGameObject()->GetComponent<CWeapon_bu>();	
	assert(m_pWeapon);
	m_pFlipObj = GetGameObject()->FindGameObjectInChilds(BUTCHER_ObjName_Flip);
	assert(m_pFlipObj);
	m_pFlipGunObj = GetGameObject()->FindGameObjectInChilds(BUTCHER_ObjName_FlipGun);
	assert(m_pFlipGunObj);
	m_pMuzzleObj = GetGameObject()->FindGameObjectInChilds(BUTCHER_ObjName_Muzzle);
	assert(m_pMuzzleObj);
	m_pGunRotationPosObj = GetGameObject()->FindGameObjectInChilds(BUTCHER_ObjName_RotationPos);
	assert(m_pGunRotationPosObj);
}

void CCharacter_bu::ChangeState(E_CharacterState _eState)
{
	if (m_CurStateEndFunc)
		m_CurStateEndFunc();
	m_eCharacterState = _eState;
	
	switch(_eState){
	case E_CharacterState::Attack:
		m_CurStateUpdateFunc = std::bind(&CCharacter_bu::OnAttackUpdate, this);
		m_CurStateStartFunc = std::bind(&CCharacter_bu::OnAttackStart, this);
		m_CurStateEndFunc = std::bind(&CCharacter_bu::OnAttackEnd, this);
		break;
	case E_CharacterState::Idle:
		m_CurStateUpdateFunc = std::bind(&CCharacter_bu::OnIdleUpdate, this);
		m_CurStateStartFunc = std::bind(&CCharacter_bu::OnIdleStart, this);
		m_CurStateEndFunc = std::bind(&CCharacter_bu::OnIdleEnd, this);

		break;
	case E_CharacterState::Wander:
		m_CurStateUpdateFunc = std::bind(&CCharacter_bu::OnWanderUpdate, this);
		m_CurStateStartFunc = std::bind(&CCharacter_bu::OnWanderStart, this);
		m_CurStateEndFunc = std::bind(&CCharacter_bu::OnWanderEnd, this);

		break;
	case E_CharacterState::Dead:
		m_CurStateUpdateFunc = std::bind(&CCharacter_bu::OnDeadUpdate, this);
		m_CurStateStartFunc = std::bind(&CCharacter_bu::OnDeadStart, this);
		m_CurStateEndFunc = std::bind(&CCharacter_bu::OnDeadEnd, this);

		break;
	case E_CharacterState::Stun:
		m_CurStateUpdateFunc = std::bind(&CCharacter_bu::OnStunUpdate, this);
		m_CurStateStartFunc = std::bind(&CCharacter_bu::OnStunStart, this);
		m_CurStateEndFunc = std::bind(&CCharacter_bu::OnStunEnd, this);

		break;
	case E_CharacterState::Runaway:
		m_CurStateUpdateFunc = std::bind(&CCharacter_bu::OnRunawayUpdate, this);
		m_CurStateStartFunc = std::bind(&CCharacter_bu::OnRunawayStart, this);
		m_CurStateEndFunc = std::bind(&CCharacter_bu::OnRunawayEnd, this);

		break;
	case E_CharacterState::Follow:
		m_CurStateUpdateFunc = std::bind(&CCharacter_bu::OnFollowUpdate, this);
		m_CurStateStartFunc = std::bind(&CCharacter_bu::OnFollowStart, this);
		m_CurStateEndFunc = std::bind(&CCharacter_bu::OnFollowEnd, this);
		break;
	case E_CharacterState::Shoot:
		m_CurStateUpdateFunc = std::bind(&CCharacter_bu::OnShootUpdate, this);
		m_CurStateStartFunc = std::bind(&CCharacter_bu::OnShootStart, this);
		m_CurStateEndFunc = std::bind(&CCharacter_bu::OnShootEnd, this);
		break;
	case E_CharacterState::Jump:
		m_CurStateUpdateFunc = std::bind(&CCharacter_bu::OnJumpUpdate, this);
		m_CurStateStartFunc = std::bind(&CCharacter_bu::OnJumpStart, this);
		m_CurStateEndFunc = std::bind(&CCharacter_bu::OnJumpEnd, this);
		break;
	case E_CharacterState::Move:
		m_CurStateUpdateFunc = std::bind(&CCharacter_bu::OnMoveUpdate, this);
		m_CurStateStartFunc = std::bind(&CCharacter_bu::OnMoveStart, this);
		m_CurStateEndFunc = std::bind(&CCharacter_bu::OnMoveEnd, this);
		break;
	default:
		assert(nullptr);
		break;
	}
	if (m_CurStateStartFunc)
		m_CurStateStartFunc();
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
	if (m_pWeapon->IsUseableWeapon(_eType))
		m_pWeapon->ChangeWeapon(_eType);
}
