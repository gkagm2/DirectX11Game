#include "pch.h"
#include "CEnemyController_bu.h"

CEnemyController_bu::CEnemyController_bu() :
	CCharacter_bu((UINT)SCRIPT_TYPE::ENEMYCONTROLLER_BU),
	m_pTargetObj(nullptr),
	m_eAIState(E_AIState_bu::Idle),
	m_pMuzzleObj(nullptr),
	m_pGunRotationPosObj(nullptr)
{
}

CEnemyController_bu::~CEnemyController_bu()
{
	AIEnd();
}

void CEnemyController_bu::Awake()
{
	m_pMuzzleObj = GetGameObject()->FindGameObjectInChilds(BUTCHER_ObjName_Muzzle);
	assert(m_pMuzzleObj);
	m_pGunRotationPosObj = GetGameObject()->FindGameObjectInChilds(BUTCHER_ObjName_RotationPos);
	assert(m_pGunRotationPosObj);
}

void CEnemyController_bu::Start()
{
	AIStart();
}

void CEnemyController_bu::Update()
{
	AIUpdate();
}

void CEnemyController_bu::AIStart()
{
	if (m_CurStateInitFunc)
		m_CurStateInitFunc();
}

void CEnemyController_bu::AIUpdate()
{
	if(m_CurStateUpdateFunc)
		m_CurStateUpdateFunc();
}

void CEnemyController_bu::AIEnd()
{
	if (m_CurStateEndFunc)
		m_CurStateEndFunc();
}

void CEnemyController_bu::ChangeState(E_AIState_bu _eState)
{
	m_CurStateEndFunc();
	m_eAIState = _eState;
	switch (m_eAIState) {
	case E_AIState_bu::Idle: {
		m_CurStateEndFunc = std::bind(&CEnemyController_bu::IdleStateEnd, this);
		m_CurStateInitFunc = std::bind(&CEnemyController_bu::IdleStateInit, this);
		m_CurStateUpdateFunc = std::bind(&CEnemyController_bu::IdleStateUpdate, this);
	}
		break;
	case E_AIState_bu::Wander:
		m_CurStateEndFunc = std::bind(&CEnemyController_bu::WanderStateEnd, this);
		m_CurStateInitFunc = std::bind(&CEnemyController_bu::WanderStateInit, this);
		m_CurStateUpdateFunc = std::bind(&CEnemyController_bu::WanderStateUpdate, this);
		break;
	case E_AIState_bu::Dead:
		m_CurStateEndFunc = std::bind(&CEnemyController_bu::DeadStateEnd, this);
		m_CurStateInitFunc = std::bind(&CEnemyController_bu::DeadStateInit, this);
		m_CurStateUpdateFunc = std::bind(&CEnemyController_bu::DeadStateUpdate, this);
		break;
	case E_AIState_bu::Stun:
		m_CurStateEndFunc = std::bind(&CEnemyController_bu::StunStateEnd, this);
		m_CurStateInitFunc = std::bind(&CEnemyController_bu::StunStateInit, this);
		m_CurStateUpdateFunc = std::bind(&CEnemyController_bu::StunStateUpdate, this);
		break;
	case E_AIState_bu::Runaway:
		m_CurStateEndFunc = std::bind(&CEnemyController_bu::RunawayStateEnd, this);
		m_CurStateInitFunc = std::bind(&CEnemyController_bu::RunawayStateInit, this);
		m_CurStateUpdateFunc = std::bind(&CEnemyController_bu::RunawayStateUpdate, this);
		break;
	case E_AIState_bu::Follow:
		m_CurStateEndFunc = std::bind(&CEnemyController_bu::FollowStateEnd, this);
		m_CurStateInitFunc = std::bind(&CEnemyController_bu::FollowStateInit, this);
		m_CurStateUpdateFunc = std::bind(&CEnemyController_bu::FollowStateUpdate, this);
		break;
	case E_AIState_bu::Shoot:
		m_CurStateEndFunc = std::bind(&CEnemyController_bu::ShootStateEnd, this);
		m_CurStateInitFunc = std::bind(&CEnemyController_bu::ShootStateInit, this);
		m_CurStateUpdateFunc = std::bind(&CEnemyController_bu::ShootStateUpdate, this);
		break;
	default:
		assert(nullptr);
		break;
	}
	
	m_CurStateInitFunc();
}


/////////////////////////////


void CEnemyController_bu::IdleStateInit()
{
}

void CEnemyController_bu::IdleStateUpdate()
{
}

void CEnemyController_bu::IdleStateEnd()
{
}

void CEnemyController_bu::WanderStateInit()
{
}

void CEnemyController_bu::WanderStateUpdate()
{
}

void CEnemyController_bu::WanderStateEnd()
{
}

void CEnemyController_bu::DeadStateInit()
{
}

void CEnemyController_bu::DeadStateUpdate()
{
}

void CEnemyController_bu::DeadStateEnd()
{
}

void CEnemyController_bu::StunStateInit()
{
}

void CEnemyController_bu::StunStateUpdate()
{
}

void CEnemyController_bu::StunStateEnd()
{
}

void CEnemyController_bu::RunawayStateInit()
{
}

void CEnemyController_bu::RunawayStateUpdate()
{
}

void CEnemyController_bu::RunawayStateEnd()
{
}

void CEnemyController_bu::FollowStateInit()
{
}

void CEnemyController_bu::FollowStateUpdate()
{
}

void CEnemyController_bu::FollowStateEnd()
{
}

void CEnemyController_bu::ShootStateInit()
{
}

void CEnemyController_bu::ShootStateUpdate()
{
}

void CEnemyController_bu::ShootStateEnd()
{
}
