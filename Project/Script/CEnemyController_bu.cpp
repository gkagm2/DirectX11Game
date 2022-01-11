#include "pch.h"
#include "CEnemyController_bu.h"
#include <Engine\CPathFind2D.h>
#include <Engine\CCollisionManager.h>

CEnemyController_bu::CEnemyController_bu() :
	CCharacter_bu((UINT)SCRIPT_TYPE::ENEMYCONTROLLER_BU),
	m_pTargetObj(nullptr),
	m_eAIState(E_AIState_bu::Idle),
	m_pMuzzleObj(nullptr),
	m_pGunRotationPosObj(nullptr),
	m_fTargetFindTime(0.f)
{
	AddParam(TScriptParam{ _T("Cur AI State"), E_ScriptParam::STRING_PRINT, &strAIStateName });
	m_pPathFind = new CPathFind2D;
	m_pPathFind->SetColRow(100, 100);
	// 장애물에 collider가 있는지 확인한다.
}

CEnemyController_bu::~CEnemyController_bu()
{
	AIEnd();
	if (m_pPathFind) {
		delete m_pPathFind;
		m_pPathFind = nullptr;
	}
}

void CEnemyController_bu::Awake()
{
	CCharacter_bu::Awake();
	m_pMuzzleObj = GetGameObject()->FindGameObjectInChilds(BUTCHER_ObjName_Muzzle);
	assert(m_pMuzzleObj);
	m_pGunRotationPosObj = GetGameObject()->FindGameObjectInChilds(BUTCHER_ObjName_RotationPos);
	assert(m_pGunRotationPosObj);
	assert(Rigidbody2D());
}

void CEnemyController_bu::Start()
{
	AIStart();
}

void CEnemyController_bu::Update()
{
	if (!m_pTargetObj) {
		m_fTargetFindTime += DT;
		if (m_fTargetFindTime > 2.f) {
			m_pTargetObj = FIND_GameObject(_T("Player"));
			m_fTargetFindTime = 0.f;
		}
	}
	
	AIUpdate();
}

void CEnemyController_bu::AIStart()
{
	ChangeAIState(E_AIState_bu::Wander);
}

void CEnemyController_bu::AIUpdate()
{
	if(m_CurAIStateUpdateFunc)
		m_CurAIStateUpdateFunc();
}

void CEnemyController_bu::AIEnd()
{
	if (m_CurAIStateEndFunc)
		m_CurAIStateEndFunc();
}

void CEnemyController_bu::ChangeAIState(E_AIState_bu _eState)
{
	if(m_CurAIStateEndFunc)
		m_CurAIStateEndFunc();
	m_eAIState = _eState;
	switch (m_eAIState) {
	case E_AIState_bu::Idle: {
		m_CurAIStateInitFunc = std::bind(&CEnemyController_bu::IdleStateInit, this);
		m_CurAIStateUpdateFunc = std::bind(&CEnemyController_bu::IdleStateUpdate, this);
		m_CurAIStateEndFunc = std::bind(&CEnemyController_bu::IdleStateEnd, this);
	}
		break;
	case E_AIState_bu::Wander:
		m_CurAIStateInitFunc = std::bind(&CEnemyController_bu::WanderStateInit, this);
		m_CurAIStateUpdateFunc = std::bind(&CEnemyController_bu::WanderStateUpdate, this);
		m_CurAIStateEndFunc = std::bind(&CEnemyController_bu::WanderStateEnd, this);
		break;
	case E_AIState_bu::Dead:
		m_CurAIStateInitFunc = std::bind(&CEnemyController_bu::DeadStateInit, this);
		m_CurAIStateUpdateFunc = std::bind(&CEnemyController_bu::DeadStateUpdate, this);
		m_CurAIStateEndFunc = std::bind(&CEnemyController_bu::DeadStateEnd, this);
		break;
	case E_AIState_bu::Stun:
		m_CurAIStateInitFunc = std::bind(&CEnemyController_bu::StunStateInit, this);
		m_CurAIStateUpdateFunc = std::bind(&CEnemyController_bu::StunStateUpdate, this);
		m_CurAIStateEndFunc = std::bind(&CEnemyController_bu::StunStateEnd, this);
		break;
	case E_AIState_bu::Runaway:
		m_CurAIStateInitFunc = std::bind(&CEnemyController_bu::RunawayStateInit, this);
		m_CurAIStateUpdateFunc = std::bind(&CEnemyController_bu::RunawayStateUpdate, this);
		m_CurAIStateEndFunc = std::bind(&CEnemyController_bu::RunawayStateEnd, this);
		break;
	case E_AIState_bu::Follow:
		m_CurAIStateInitFunc = std::bind(&CEnemyController_bu::FollowStateInit, this);
		m_CurAIStateUpdateFunc = std::bind(&CEnemyController_bu::FollowStateUpdate, this);
		m_CurAIStateEndFunc = std::bind(&CEnemyController_bu::FollowStateEnd, this);
		break;
	case E_AIState_bu::Shoot:
		m_CurAIStateInitFunc = std::bind(&CEnemyController_bu::ShootStateInit, this);
		m_CurAIStateUpdateFunc = std::bind(&CEnemyController_bu::ShootStateUpdate, this);
		m_CurAIStateEndFunc = std::bind(&CEnemyController_bu::ShootStateEnd, this);
		break;
	default:
		assert(nullptr);
		break;
	}

	if(m_CurAIStateInitFunc)
		m_CurAIStateInitFunc();
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
	if (!m_pTargetObj)
		return;
	// path finding test
	Vector3 vTargetPos = m_pTargetObj->Transform()->GetPosition();
	Vector2 vTargetPos2 = Vector2(vTargetPos.x, vTargetPos.y);
	Vector3 vPos = Transform()->GetPosition();
	Vector2 vPos2 = Vector2(vPos.x, vPos.y);
	if (!m_pPathFind)
		return;
	static float ftime = 0.f;
	static bool isFindPath = false;
	ftime += DT;
	
	if (ftime > 0.2f) {
		isFindPath = m_pPathFind->FindPath(vTargetPos2, vPos2);
		ftime = 0.f;
	}

	if (isFindPath) {
		list<Vector2>& listNextPath = m_pPathFind->GetPath();
		if (!listNextPath.empty()) {
			Vector2 vNextPath = m_pPathFind->GetNextPath();
			Vector3 vNextPos = Vector3(vNextPath.x, vNextPath.y, vPos.z);
			Vector3 vMoveDir = vPos - vNextPos;
			vMoveDir.Normalize();
			Vector3 vForce = vMoveDir * m_fMovePower;
			Rigidbody2D()->AddForce(vForce);
		}
	}
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
