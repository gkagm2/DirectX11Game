#include "pch.h"
#include "CCharacter_bu.h"
#include "CWeapon_bu.h"
#include "CGroundCheck_bu.h"

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
	m_pMuzzleParticleObj{ nullptr },
	m_pGroundCheckObj{ nullptr },
	m_pGroundCheckCol{ nullptr },
	m_bCanJump{ false },
	m_fJumpCoolTime{ 0.f },
	m_fJumpMaxCoolTime{0.3f},
	m_pBodyPartPref{ nullptr },
	m_fFootstepSoundDelTime{ 0.f },
	m_fMaxFootstepSoundDelTime{0.3f}
{
}

CCharacter_bu::CCharacter_bu(UINT _iScriptType) :
	CScript(_iScriptType),
	m_fJumpPower{ 0.5f },
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
	m_pMuzzleParticleObj{ nullptr },
	m_pFlipObj{nullptr},
	m_pFlipGunObj{ nullptr },
	m_pGroundCheckObj{ nullptr },
	m_pGroundCheckCol{ nullptr },
	m_bCanJump{false},
	m_fJumpCoolTime{ 0.f },
	m_fJumpMaxCoolTime{ 0.3f },
	m_pBodyPartPref{ nullptr },
	m_fFootstepSoundDelTime{ 0.f },
	m_fMaxFootstepSoundDelTime{ 0.3f }
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
	m_pMuzzleParticleObj = m_pMuzzleObj->FindGameObjectInChilds(BUTCHER_ObjName_MuzzleParticle);
	assert(m_pMuzzleParticleObj);
	m_pGunRotationPosObj = GetGameObject()->FindGameObjectInChilds(BUTCHER_ObjName_RotationPos);
	assert(m_pGunRotationPosObj);
	m_pGroundCheckObj = GetGameObject()->FindGameObjectInChilds(BUTCHER_ObjName_GroundCheck);
	assert(m_pGroundCheckObj);
	m_pGroundCheckCol = m_pGroundCheckObj->GetComponent<CGroundCheck_bu>();
	assert(m_pGroundCheckCol);

	tstring strPath = STR_DIR_PATH_Prefab;
	strPath = strPath +_T("BodyPart_bu") + STR_EXTENSION_Prefab;
	m_pBodyPartPref = CResourceManager::GetInstance()->FindRes<CPrefab>(strPath);
	if (nullptr == m_pBodyPartPref) {
		m_pBodyPartPref = CResourceManager::GetInstance()->FindRes<CPrefab>(_T("BodyPart_bu"));
		if (nullptr == m_pBodyPartPref)
			assert(nullptr);
	}
}

void CCharacter_bu::PrevUpdate()
{
	if (m_pGroundCheckCol->IsTouchGround())
		m_bCanJump = true;
	else
		m_bCanJump = false;
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
	float fDamage = _fDamage;
	if (m_fArmor > 0.f) {
		if (m_fArmor >= _fDamage) {
			m_fArmor = m_fArmor - _fDamage;
			fDamage = 0.f;
		}
		else {// 데미지가 더 크면
			fDamage = fabsf(_fDamage - m_fArmor);
			m_fArmor = 0.f;
		}
	}
	if (fDamage > 0) {
		m_fHp = m_fHp - fDamage;
		m_fHp = max(m_fHp, 0.f);
	}

	if (0.f == m_fHp) {
		ChangeState(E_CharacterState::Dead);
	}
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

void CCharacter_bu::OnCollisionStay2D(CCollider2D* _pCol)
{
}