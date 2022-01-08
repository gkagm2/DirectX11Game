#pragma once
#include "CCharacter_Bu.h"
#include <Engine\CPathFind2D.h>
enum class E_AIState_bu {
	Idle,
	Wander,
	Dead,
	Stun,
	Runaway,
	Follow,
	Shoot
};

class CEnemyController_bu : public CCharacter_bu
{
private:
	CGameObject* m_pTargetObj;
	E_AIState_bu m_eAIState;
	tstring strAIStateName;

	CGameObject* m_pMuzzleObj;
	CGameObject* m_pGunRotationPosObj;


	std::function<void()> m_CurStateInitFunc;
	std::function<void()> m_CurStateUpdateFunc;
	std::function<void()> m_CurStateEndFunc;

	CPathFind2D* m_pPathFind;

public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;

public:
	// AI Functions
	void ChangeAIState(E_AIState_bu _eState);
	void AIStart();
	void AIUpdate();
	void AIEnd();

public:
	// AI States
	void IdleStateInit();
	void IdleStateUpdate();
	void IdleStateEnd();

	void WanderStateInit();
	void WanderStateUpdate();
	void WanderStateEnd();

	void DeadStateInit();
	void DeadStateUpdate();
	void DeadStateEnd();

	void StunStateInit();
	void StunStateUpdate();
	void StunStateEnd();

	void RunawayStateInit();
	void RunawayStateUpdate();
	void RunawayStateEnd();

	void FollowStateInit();
	void FollowStateUpdate();
	void FollowStateEnd();

	void ShootStateInit();
	void ShootStateUpdate();
	void ShootStateEnd();

public:
	CLONE(CEnemyController_bu);
	CEnemyController_bu();
	virtual ~CEnemyController_bu() override;
};