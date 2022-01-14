#include "pch.h"
#include "CEnemyController_bu.h"
#include <Engine\CPathFind2D.h>
#include <Engine\CCollisionManager.h>
#include <Engine\CAnimator2D.h>
#include <Engine\CObjectManager.h>
#include "CTargetLookAt_bu.h"

CEnemyController_bu::CEnemyController_bu() :
	CCharacter_bu((UINT)SCRIPT_TYPE::ENEMYCONTROLLER_BU),
	m_pTargetObj(nullptr),
	m_eAIState(E_AIState_bu::Wander),
	m_fTargetFindTime(1.f),
	m_fMaxTargetFindTime(1.0f),
	m_pEmoticonObj{ nullptr },
	m_pEmoticonAnim{ nullptr },
	m_pTargetLookAt{ nullptr }
{
	AddParam(TScriptParam{ _T("Cur AI State"), E_ScriptParam::STRING_PRINT, &strAIStateName });
	m_pPathFind = new CPathFind2D;
	m_pPathFind->SetColRow(150, 150);
	m_pPathFind->SetOriginPos(Vector2(-75.f, -75.f));
	// 장애물에 collider가 있는지 확인한다.
}

CEnemyController_bu::CEnemyController_bu(const CEnemyController_bu& _origin) :
	CCharacter_bu((UINT)SCRIPT_TYPE::ENEMYCONTROLLER_BU),
	m_pTargetObj(nullptr),
	m_eAIState(_origin.m_eAIState),
	m_fTargetFindTime(_origin.m_fTargetFindTime),
	m_fMaxTargetFindTime(_origin.m_fMaxTargetFindTime),
	m_pEmoticonObj{ nullptr },
	m_pEmoticonAnim{ nullptr },
	m_pTargetLookAt{ nullptr }
{
	AddParam(TScriptParam{ _T("Cur AI State"), E_ScriptParam::STRING_PRINT, &strAIStateName });
	m_pPathFind = new CPathFind2D;
	m_pPathFind->SetColRow(150, 150);
	m_pPathFind->SetOriginPos(Vector2(-75.f, -75.f));
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

	CGameObject* pLegsObj = GetGameObject()->FindGameObjectInChilds(BUTCHER_ObjName_Legs);
	assert(pLegsObj);
	CGameObject* pTorsoObj = GetGameObject()->FindGameObjectInChilds(BUTCHER_ObjName_Torse);
	assert(pTorsoObj);
	if (pLegsObj)
		m_pLegAnim = pLegsObj->Animator2D();
	if (pTorsoObj)
		m_pTorsoAnimSprite = pTorsoObj->Animator2D();
	assert(m_pLegAnim);
	assert(m_pTorsoAnimSprite);

	assert(Rigidbody2D());
	m_pEmoticonObj = GetGameObject()->FindGameObjectInChilds(BUTCHER_ObjName_Emoticon);
	assert(m_pEmoticonObj);
	m_pEmoticonAnim = m_pEmoticonObj->Animator2D();
	assert(m_pEmoticonAnim);

	m_pTargetLookAt = GetGameObject()->FindComponentInChilds<CTargetLookAt_bu>();
	assert(m_pTargetLookAt);
}

void CEnemyController_bu::Start()
{
	vector<CGameObject*> vecWallObjs;
	CSceneManager::GetInstance()->GetCurScene()->GetGameObjects(vecWallObjs, (UINT)E_Layer::Object);
	for (int i = -75; i < 75; ++i) {
		for (int j = -75; j < 75; ++j) {
			for (int m = 0; m < vecWallObjs.size(); ++m) {
				if (vecWallObjs[m]->GetTag() == (UINT)E_Tag::Wall) {
					bool isCol = CCollisionManager::GetInstance()->IsCollision(vecWallObjs[m]->Collider2D(), Vector3(j, i, 0));
					if (isCol) {
						if (!m_pPathFind->IsObstacle(Vector2(j, i))) {
							m_pPathFind->AddObstacleTile(Vector2(j, i));
							//CGameObject* pObj = CObjectManager::GetInstance()->Create2DRectGameObject();
							//pObj->Transform()->SetLocalPosition(Vector3(j, i, 0));
							break;
						}
					}
				}
			}
		}
	}

	AIStart();
}

void CEnemyController_bu::Update()
{
	if (!m_pTargetObj) {
		m_fTargetFindTime += DT;
		if (m_fTargetFindTime > m_fMaxTargetFindTime) {
			m_pTargetObj = FIND_GameObject(_T("Player"));
			m_fTargetFindTime = 0.f;
		}
	}

	// 각도에 따라서 상체 애니메이션을 다르게 한다.
	
	CAnimation2D* pAnim = m_pTorsoAnimSprite->GetCurAnimation();
	if (pAnim) {
		UINT iSize = (UINT)pAnim->GetAnimationFrame().size();
		// Rotation angle을 본다.
		float fDegree = m_pGunRotationPosObj->Transform()->GetRotationDegree().z;
		int idx = 0;
		float fDegr = 180.f / iSize;
		if (fDegree >= 0 && fDegree <= 90.f) {
			float degree = 90.f - fDegree;
			idx = (int)(degree / fDegr);

		}
		else if (fDegree > 90.f) {
			fDegree -= 90.f;
			float degree = fDegree;
			idx = (int)(degree / fDegr);
		}
		else if (fDegree < 0 && fDegree >= -90) {
			fDegree = fabsf(fDegree);
			float degree = fDegree;
			idx = (int)(degree / fDegr);
			idx = (int)(iSize * 0.5f) + idx;
		}
		else {
			fDegree = fabsf(fDegree);
			float degree = 180.f - fDegree;
			idx = (int)(degree / fDegr);
			idx = (int)(iSize * 0.5f) + idx;
		}
		idx = CMyMath::Clamp(idx, 0, max(0, (int(iSize) - 1)));
		m_pTorsoAnimSprite->SetCurAnimationFrame(idx);
	}

	// Flip
	if (m_bIsTargeting) {
		float fDegree = m_pGunRotationPosObj->Transform()->GetRotationDegree().z;
		if (fDegree <= 90.f && fDegree >= -90.f)
			m_bIsLookRight = true;
		else
			m_bIsLookRight = false;
	}

	Vector3 vScale = m_pFlipObj->Transform()->GetLocalScale();
	vScale.x = m_bIsLookRight == true ? 1.f : -1.f;
	m_pFlipObj->Transform()->SetLocalScale(vScale);

	if (m_bIsLookRight) {
		m_pFlipGunObj->Transform()->GetLocalScale();
		Vector3 vRotGun = m_pFlipGunObj->Transform()->GetLocalRotation();
		vRotGun.z = 0.f;
		Vector3 vScale = m_pFlipGunObj->Transform()->GetLocalScale();
		vScale.x = 1.f;
		m_pFlipGunObj->Transform()->SetLocalRotation(vRotGun);
		m_pFlipGunObj->Transform()->SetLocalScale(vScale);

	}
	else {
		m_pFlipGunObj->Transform()->GetLocalScale();
		Vector3 vRotGun = m_pFlipGunObj->Transform()->GetLocalRotation();
		vRotGun.z = PI;
		Vector3 vScale = m_pFlipGunObj->Transform()->GetLocalScale();
		vScale.x = -1.f;
		m_pFlipGunObj->Transform()->SetLocalRotation(vRotGun);
		m_pFlipGunObj->Transform()->SetLocalScale(vScale);
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
	case E_AIState_bu::Notice:
		m_CurAIStateInitFunc = std::bind(&CEnemyController_bu::NoticeStateInit, this);
		m_CurAIStateUpdateFunc = std::bind(&CEnemyController_bu::NoticeStateUpdate, this);
		m_CurAIStateEndFunc = std::bind(&CEnemyController_bu::NoticeStateEnd, this);
		break;
	case E_AIState_bu::Bewildered:
		m_CurAIStateInitFunc = std::bind(&CEnemyController_bu::BewilderedStateInit, this);
		m_CurAIStateUpdateFunc = std::bind(&CEnemyController_bu::BewilderedStateUpdate, this);
		m_CurAIStateEndFunc = std::bind(&CEnemyController_bu::BewilderedStateEnd, this);
		break;
	default:
		assert(nullptr);
		break;
	}
	strAIStateName = AIStateToStr_bu(m_eAIState);

	if(m_CurAIStateInitFunc)
		m_CurAIStateInitFunc();
}


/////////////////////////////
int iCurWayPointIdx;
bool isFindPath = false;
void CEnemyController_bu::WanderStateInit()
{
	m_fTargetFindTime = m_fMaxTargetFindTime;
	iCurWayPointIdx = 0;
	isFindPath = false;
	m_pLegAnim->Play(_T("Enemy_Walk_bu"));
}

void CEnemyController_bu::WanderStateUpdate()
{
	static CGameObject* pWPObj = FIND_GameObject(BUTCHER_ObjName_WayPoint_bu);
	if (pWPObj) {
		vector<CGameObject*>& vecWayPoints = pWPObj->GetChildsObject();
		if (vecWayPoints.empty()) {
			//ChangeState(E_CharacterState::Idle);
			return;
		}
	}
	else {
		pWPObj = FIND_GameObject(BUTCHER_ObjName_WayPoint_bu);
		//ChangeState(E_CharacterState::Idle);
		return;
	}
	vector<CGameObject*>& vecWayPoints = pWPObj->GetChildsObject();

	// path finding test
	
	Vector3 vMyPos = Transform()->GetPosition();
	if (!m_pPathFind)
		return;
	bool isArriveDestination = false;
	m_fTargetFindTime += DT;

	if(m_fTargetFindTime > m_fMaxTargetFindTime) {
		Vector3 vTargetPos = vecWayPoints[iCurWayPointIdx]->Transform()->GetPosition();

		m_pPathFind->FindPath(vMyPos.XY(), vTargetPos.XY());
		m_fTargetFindTime = 0.f;
	}
		
	list<Vector2>& listNextPath = m_pPathFind->GetPath();
	if (!listNextPath.empty()) {
		Vector2 vNextPath = m_pPathFind->GetNextPath();
		Vector3 vNextPos = Vector3(vNextPath.x, vNextPath.y, vMyPos.z);
		Vector3 vMoveDir = vNextPos - vMyPos;
		vMoveDir.Normalize();
		Vector3 vForce = vMoveDir * m_fMovePower;
		Rigidbody2D()->AddForce(vForce);

		if (vForce.x > 0)
			_SetLookRightState(true);
		else
			_SetLookRightState(false);
		float vDis = Vector2::Distance(vNextPath, Transform()->GetPosition().XY());
		if (vDis < 0.7f) {
			listNextPath.pop_front();
		}
	}
	else {
		++iCurWayPointIdx;
		iCurWayPointIdx = (iCurWayPointIdx) % vecWayPoints.size();
		Vector3 vTargetPos = vecWayPoints[iCurWayPointIdx]->Transform()->GetPosition();
		
		m_pPathFind->FindPath(vMyPos.XY(), vTargetPos.XY());
		m_fTargetFindTime = 0.f;
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

float fStunTime = 0.f;
float fMaxStunTime = 1.f;
void CEnemyController_bu::StunStateInit()
{
	m_pEmoticonObj->SetActive(true);
	m_pEmoticonAnim->Play(BUTCHER_AnimName_EmoticonStun, E_AnimationState::Once, true);
	
	fStunTime = 0.f;
	fMaxStunTime = 1.f;

	m_pLegAnim->Play(_T("Enemy_Idle_bu"));
}

void CEnemyController_bu::StunStateUpdate()
{
	fStunTime += DT;
	if (fStunTime > fMaxStunTime) {
		ChangeAIState(E_AIState_bu::Follow);
	}
}

void CEnemyController_bu::StunStateEnd()
{
	m_pEmoticonObj->SetActive(false);
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

float fDetectRange = 5.f;
bool isKeepGoToTargetPos = false;
Vector2 vNextPath;
void CEnemyController_bu::FollowStateInit()
{
	fDetectRange = 5.f;
	m_fTargetFindTime = m_fMaxTargetFindTime;
	isKeepGoToTargetPos = false;

	// targetObject 세팅
	// target look at의 타겟을 세팅
}

void CEnemyController_bu::FollowStateUpdate()
{
	Vector2 vTargetPos = m_pTargetObj->Transform()->GetPosition().XY();
	Vector2 vMyPos = Transform()->GetPosition().XY();

	float fLength = Vector2::Distance(vTargetPos, vMyPos);

	bool bKeepFollow = true;
	if (fLength > fDetectRange)
		bKeepFollow = false;

	if (!bKeepFollow)
		ChangeAIState(E_AIState_bu::Bewildered);
	else {
		bool isPathFound = false;

		if (m_fTargetFindTime > m_fMaxTargetFindTime) {
			isPathFound = m_pPathFind->FindPath(vMyPos, vTargetPos);
			m_fTargetFindTime = 0.f;
		}

		if (isPathFound) {
			vNextPath = m_pPathFind->GetNextPath();
			isKeepGoToTargetPos = true;
		}
		else
			isKeepGoToTargetPos = false;

		if (isKeepGoToTargetPos) {
			Vector2 vForce2DDir = vNextPath - vMyPos;
			Vector3 vForceDir = Vector3(vForce2DDir.x, 0.f, 0.f);
			vForceDir.Normalize();
			Rigidbody2D()->AddForce(vForceDir * m_fMovePower);
		}
		else {
			ChangeAIState(E_AIState_bu::Bewildered);
		}

		ChangeState(E_CharacterState::Shoot);

	}

}

void CEnemyController_bu::FollowStateEnd()
{
}

float fKeepNoticeTime = 0.f;
float fMaxKeepNoticeTime = 1.f;
void CEnemyController_bu::NoticeStateInit()
{
	m_pEmoticonObj->SetActive(true);
	m_pEmoticonAnim->Play(BUTCHER_AnimName_EmoticonSuprise, E_AnimationState::Once, true);

	fKeepNoticeTime = 0.f;
	fMaxKeepNoticeTime = 1.f;
}

void CEnemyController_bu::NoticeStateUpdate()
{
	fKeepNoticeTime += DT;
	if (fKeepNoticeTime >= fMaxKeepNoticeTime)
		ChangeAIState(E_AIState_bu::Follow);
}

void CEnemyController_bu::NoticeStateEnd()
{
	m_pEmoticonObj->SetActive(false);
}

float fBewilderedTime = 0.f;
float fBewilderedMaxTime = 2.f;
void CEnemyController_bu::BewilderedStateInit()
{
	m_pEmoticonObj->SetActive(true);
	m_pEmoticonAnim->Play(BUTCHER_AnimName_EmoticonQuestionMark, E_AnimationState::Loop, true);

	fBewilderedTime = 0.f;
	fBewilderedMaxTime = 2.f;

}

void CEnemyController_bu::BewilderedStateUpdate()
{
	if(fBewilderedTime > fBewilderedMaxTime)
		ChangeAIState(E_AIState_bu::Wander);
}

void CEnemyController_bu::BewilderedStateEnd()
{
	m_pEmoticonObj->SetActive(false);
}

void CEnemyController_bu::OnShootStart()
{
}

void CEnemyController_bu::OnShootUpdate()
{
	// 누굴 겨냥해야되는건진 알고있나?
	Vector3 vmuzzlePos = m_pMuzzleObj->Transform()->GetPosition();
	Vector3 vrotPos = m_pGunRotationPosObj->Transform()->GetRotation();
	Vector3 vfrontVec = m_pGunRotationPosObj->Transform()->GetRightVector();
	UINT iLayer = (UINT)E_Tag::Enemy_Bullet;
	m_pWeapon->Fire(vmuzzlePos, vrotPos, vfrontVec, iLayer);
}

void CEnemyController_bu::OnIdleStart()
{
	m_pLegAnim->Play(_T("Enemy_Idle_bu"), E_AnimationState::Loop);
}

void CEnemyController_bu::OnIdleUpdate()
{

}

void CEnemyController_bu::OnIdleEnd()
{
}

void CEnemyController_bu::OnShootEnd()
{
}

tstring AIStateToStr_bu(E_AIState_bu _eState)
{
	tstring strName;
	switch (_eState) {
	case E_AIState_bu::Wander:
		strName = _T("Wander");
		break;
	case E_AIState_bu::Dead:
		strName = _T("Dead");
		break;
	case E_AIState_bu::Stun:
		strName = _T("Stun");
		break;
	case E_AIState_bu::Runaway:
		strName = _T("Runaway");
		break;
	case E_AIState_bu::Follow:
		strName = _T("Follow");
		break;
	case E_AIState_bu::Notice:
		strName = _T("Notice");
		break;
	case E_AIState_bu::Bewildered:
		strName = _T("Bewildered");
		break;
	}
	return strName;
}
