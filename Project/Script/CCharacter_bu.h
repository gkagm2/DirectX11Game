#pragma once
#include <Engine\CScript.h>
#include "CWeapon_bu.h"
class CWeapon_bu;
class CGroundCheck_bu;

enum class E_CharacterState {
	Attack,
	Idle,
	Wander,
	Dead,
	Stun,
	Runaway,
	Follow,
	Shoot,
	Jump,
	Move,
};

class CCharacter_bu : public CScript
{
protected:
	float m_fJumpPower;
	float m_fMovePower;
	Vector3 m_vCurMoveDir;

	CGameObject* m_pMuzzleObj;
	CGameObject* m_pMuzzleParticleObj;
	CGameObject* m_pGunRotationPosObj;

	float m_fHp;
	float m_fMaxHp;
	float m_fArmor;
	float m_fMaxArmor;

	CGameObject* m_pChainSawObj;
	CGameObject* m_pGunImageObj;
	CWeapon_bu* m_pWeapon;
	E_CharacterState m_eCharacterState;
	bool m_bIsLookRight;

	CGameObject* m_pFlipObj;
	CGameObject* m_pFlipGunObj;

	CGameObject* m_pGroundCheckObj;
	CGroundCheck_bu* m_pGroundCheckCol;
	bool m_bCanJump;
	bool m_bJump;
	float m_fJumpCoolTime;
	float m_fJumpMaxCoolTime;

public:
	virtual void Awake() override;
	virtual void PrevUpdate() override;

public:
	std::function<void()> m_CurStateStartFunc;
	std::function<void()> m_CurStateUpdateFunc;
	std::function<void()> m_CurStateEndFunc;

	virtual void Interaction() {}

	virtual void OnAttackStart() {}
	virtual void OnAttackUpdate() {}
	virtual void OnAttackEnd() {}
	virtual void OnIdleStart() {}
	virtual void OnIdleUpdate() {}
	virtual void OnIdleEnd() {}
	virtual void OnWanderStart() {}
	virtual void OnWanderUpdate() {}
	virtual void OnWanderEnd() {}
	virtual void OnDeadStart() {}
	virtual void OnDeadUpdate() {}
	virtual void OnDeadEnd() {}
	virtual void OnStunStart() {}
	virtual void OnStunUpdate() {}
	virtual void OnStunEnd() {}
	virtual void OnRunawayStart() {}
	virtual void OnRunawayUpdate() {}
	virtual void OnRunawayEnd() {}
	virtual void OnFollowStart() {}
	virtual void OnFollowUpdate() {}
	virtual void OnFollowEnd() {}
	virtual void OnShootStart() {}
	virtual void OnShootUpdate() {}
	virtual void OnShootEnd() {}
	virtual void OnJumpStart() {}
	virtual void OnJumpUpdate() {}
	virtual void OnJumpEnd() {}
	virtual void OnMoveStart() {}
	virtual void OnMoveUpdate() {}
	virtual void OnMoveEnd() {}

	void ChangeState(E_CharacterState _eState);

	virtual void DamagedMe(float _fDamage);

	virtual void OnDead(); // Á×À½ ½ÃÀÛ
	void ChangeWeapon(E_WeaponType_bu _eType);

public:
	float GetHp() { return m_fHp; }
	float GetArmor() { return m_fArmor; }
	float GetMaxHp() { return m_fMaxHp; }
	float GetMaxArmor() { return m_fMaxArmor; }
	void SetHp(float _fHp) { m_fHp = _fHp; }
	void SetArmor(float _fArmor) { m_fArmor = _fArmor; }
	CWeapon_bu* GetWeapon() { return m_pWeapon; }
	
	bool IsLookRight() { return m_bIsLookRight; }

	virtual void OnCollisionStay2D(CCollider2D* _pCol);

public:
	CGameObject* GetMuzzleObj() { return m_pMuzzleObj; }
	CGameObject* GetMuzzleParticleObj() { return m_pMuzzleParticleObj; }
	CGameObject* GetGunRotationPosObj() { return m_pGunRotationPosObj; }

public:
	CLONE(CCharacter_bu);
	CCharacter_bu();
	CCharacter_bu(UINT _iScriptType);
	virtual ~CCharacter_bu() override;
};