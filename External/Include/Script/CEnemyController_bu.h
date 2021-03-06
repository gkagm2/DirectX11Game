#pragma once
#include "CCharacter_Bu.h"
#include <Engine\CPathFind2D.h>
enum class E_AIState_bu {
	Wander,
	Dead,
	Stun,
	Runaway,
	Follow,
	Notice,
	Bewildered,
};
tstring AIStateToStr_bu(E_AIState_bu _eState);

class CSoundManager_bu;
class CGameManager_bu;
class CEnemyController_bu : public CCharacter_bu
{
private:
	CSoundManager_bu* m_pSoundMgr;
	CGameManager_bu* m_pGameMgr;
	CAnimator2D* m_pLegAnim;
	CAnimator2D* m_pTorsoAnimSprite;

	CGameObject* m_pTargetObj;
	E_AIState_bu m_eAIState;
	tstring strAIStateName;

	CGameObject* m_pEmoticonObj;
	CAnimator2D* m_pEmoticonAnim;

	class CTargetLookAt_bu* m_pTargetLookAt;
	bool m_bIsTargeting;


	std::function<void()> m_CurAIStateInitFunc;
	std::function<void()> m_CurAIStateUpdateFunc;
	std::function<void()> m_CurAIStateEndFunc;

	CPathFind2D* m_pPathFind;
	CGameObject* m_pWayPoint;

	float m_fTargetFindTime;
	float m_fMaxTargetFindTime;

	float m_fMaxDetectDistance;

public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;

public:
	virtual void DamagedMe(float _fDamage) override;
	void Jump();

public:
	// AI Functions
	void ChangeAIState(E_AIState_bu _eState);
	E_AIState_bu GetCurAIState() { return m_eAIState; }
	void AIStart();
	void AIUpdate();
	void AIEnd();

public:
	// AI States
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

	void NoticeStateInit();
	void NoticeStateUpdate();
	void NoticeStateEnd();

	void BewilderedStateInit();
	void BewilderedStateUpdate();
	void BewilderedStateEnd();

	virtual void OnIdleStart() override;
	virtual void OnIdleUpdate() override;
	virtual void OnIdleEnd() override;

	virtual void OnMoveStart() override;
	virtual void OnMoveUpdate() override;
	virtual void OnMoveEnd() override;

	virtual void OnJumpStart() override;
	virtual void OnJumpUpdate() override;
	virtual void OnJumpEnd() override;

	virtual void OnDeadStart() override;
	virtual void OnDeadUpdate() override;
	virtual void OnDeadEnd() override;

private:
	// Targeting???????? ?????? ???? ????
	void _SetLookRightState(bool _bIsRight) { m_bIsLookRight = _bIsRight; }

public:
	CLONE(CEnemyController_bu);
	CEnemyController_bu();
	CEnemyController_bu(const CEnemyController_bu& _origin);
	virtual ~CEnemyController_bu() override;
};