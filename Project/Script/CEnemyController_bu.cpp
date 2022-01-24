#include "pch.h"
#include "CEnemyController_bu.h"
#include <Engine\CPathFind2D.h>
#include <Engine\CCollisionManager.h>
#include <Engine\CAnimator2D.h>
#include <Engine\CObjectManager.h>
#include "CExplosion_bu.h"
#include "CTargetLookAt_bu.h"
#include "CSoundManager_bu.h"
#include "CGameManager_bu.h"

int iPathFindCol = 120;
int iPathFindRow = 35;

CEnemyController_bu::CEnemyController_bu() :
	CCharacter_bu((UINT)SCRIPT_TYPE::ENEMYCONTROLLER_BU),
	m_pTargetObj{},
	m_eAIState(E_AIState_bu::Wander),
	m_fTargetFindTime(1.f),
	m_fMaxTargetFindTime(0.6f),
	m_pEmoticonObj{ nullptr },
	m_pEmoticonAnim{ nullptr },
	m_pTargetLookAt{ nullptr },
	m_fMaxDetectDistance{ 7.f }
{
	AddParam(TScriptParam{ _T("Cur AI State"), E_ScriptParam::STRING_PRINT, &strAIStateName });
	m_pPathFind = new CPathFind2D;
	m_pPathFind->SetColRow(iPathFindCol, iPathFindRow);
	m_pPathFind->SetOriginPos(Vector2(0.5f, 0.5f));
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
	m_pTargetLookAt{ nullptr },
	m_fMaxDetectDistance{ 7.f }
{
	AddParam(TScriptParam{ _T("Cur AI State"), E_ScriptParam::STRING_PRINT, &strAIStateName });
	m_pPathFind = new CPathFind2D;
	m_pPathFind->SetColRow(iPathFindCol, iPathFindRow);
	m_pPathFind->SetOriginPos(Vector2(0.5f, 0.5f));
	// 장애물에 collider가 있는지 확인한다.
}

CEnemyController_bu::~CEnemyController_bu()
{
	//AIEnd();
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

	m_pTargetObj = FIND_GameObject(_T("Player"));
	assert(m_pTargetObj);

	vector<CGameObject*> vecWallObjs;
	CSceneManager::GetInstance()->GetCurScene()->GetGameObjects(vecWallObjs, (UINT)E_Layer::Wall);
	for (int i = 0; i < iPathFindRow; ++i) {
		for (int j = 0; j < iPathFindCol; ++j) {
			for (int m = 0; m < vecWallObjs.size(); ++m) {
				if (vecWallObjs[m]->Collider2D()) {
					bool isCol = CCollisionManager::GetInstance()->IsCollision(vecWallObjs[m]->Collider2D(), Vector3(j, i, 0));
					if (isCol) {
						if (!m_pPathFind->IsObstacle(Vector2(j, i))) {
							m_pPathFind->AddObstacleTile(Vector2(j, i));
							break;
						}
					}
				}
			}
		}
	}
}

void CEnemyController_bu::Start()
{
	SetHp(20.f);
	SetArmor(0.f);

	AIStart();

	TWeaponInfo_bu& tWeaponInfo =  m_pWeapon->GetCurWeapon();
	tWeaponInfo.bInfinity = true;
	if (E_WeaponType_bu::Chainsaw != GetWeapon()->GetCurWeaponType()) {
		m_fMovePower = m_fMovePower * 0.5f;
	}

	CGameObject* pSoundMgrObj = FIND_GameObject(_T("SoundManager"));
	if (pSoundMgrObj)
		m_pSoundMgr = pSoundMgrObj->GetComponent<CSoundManager_bu>();

	GetWeapon()->SetVolume(1.f);
	CGameObject* pGameMgrObj = FIND_GameObject(_T("GameManager"));
	if (pGameMgrObj)
		m_pGameMgr = pGameMgrObj->GetComponent<CGameManager_bu>();
}

void CEnemyController_bu::Update()
{
	if(m_pGameMgr && m_pGameMgr->GetGameMode() != E_GameMode_bu::Play)
		return;

	if (m_pTargetObj == nullptr || m_pTargetObj->IsDead())
		m_pTargetObj = nullptr;

	bool isMove = false;

	Vector3 vForce = Rigidbody2D()->GetForce();
	if (vForce != Vector3::Zero) {
		isMove = true;
	}
	else
		isMove = false;
	
	if (m_bJump) {
		m_fJumpCoolTime += DT;
		if (m_fJumpCoolTime <= m_fJumpMaxCoolTime) {
			{
				Vector3 vJumpPower = Vector3(0.f, 1.f, 0.f);
				vJumpPower *= m_fJumpPower;
				Rigidbody2D()->AddForce(vJumpPower);
			}
		}
		else
			m_bJump = false;
	}
	if (m_bCanJump)
		Rigidbody2D()->UseGravity(false);
	else
		Rigidbody2D()->UseGravity(true);

	if (isMove) {
		ChangeState(E_CharacterState::Move);
	}
	else
		ChangeState(E_CharacterState::Idle);



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
		Vector3 vRotGun = m_pFlipGunObj->Transform()->GetLocalRotationDegree();
		vRotGun.z = 0.f;
		Vector3 vScale = m_pFlipGunObj->Transform()->GetLocalScale();
		vScale.x = 1.f;
		m_pFlipGunObj->Transform()->SetLocalRotationDegree(vRotGun);
		m_pFlipGunObj->Transform()->SetLocalScale(vScale);

	}
	else {
		m_pFlipGunObj->Transform()->GetLocalScale();
		Vector3 vRotGun = m_pFlipGunObj->Transform()->GetLocalRotationDegree();
		vRotGun.z = 180.f;
		Vector3 vScale = m_pFlipGunObj->Transform()->GetLocalScale();
		vScale.x = -1.f;
		m_pFlipGunObj->Transform()->SetLocalRotationDegree(vRotGun);
		m_pFlipGunObj->Transform()->SetLocalScale(vScale);
	}

	AIUpdate();
}

void CEnemyController_bu::DamagedMe(float _fDamage)
{
	CCharacter_bu::DamagedMe(_fDamage);
	if (m_fHp > 0.f)
		ChangeAIState(E_AIState_bu::Stun);
}

void CEnemyController_bu::Jump()
{
	if (m_bCanJump) {
		m_bJump = true;
		ChangeState(E_CharacterState::Jump);
		m_fJumpCoolTime = 0.f;
	}
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
	ChangeState(E_CharacterState::Move);
}

void CEnemyController_bu::WanderStateUpdate()
{
	static CGameObject* pWPObj = FIND_GameObject(BUTCHER_ObjName_WayPoint_bu);
	if (pWPObj) {
		vector<CGameObject*>& vecWayPoints = pWPObj->GetChildsObject();
		if (vecWayPoints.empty()) {
			//ChangeState(E_CharacterState::Idle);
		}
	}
	else {
		pWPObj = FIND_GameObject(BUTCHER_ObjName_WayPoint_bu);
		//ChangeState(E_CharacterState::Idle);
	}
	if (pWPObj) {
		vector<CGameObject*>& vecWayPoints = pWPObj->GetChildsObject();
		if (!vecWayPoints.empty()) {
			Vector3 vMyPos = Transform()->GetPosition();
			if (!m_pPathFind)
				return;
			bool isArriveDestination = false;
			m_fTargetFindTime += DT;

			if (m_fTargetFindTime > m_fMaxTargetFindTime) {
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

				float fDegree = CMyMath::VectorToDegree(vMoveDir.XY());
				bool bLookingUp = fDegree >= 45.f && fDegree <= 135.f;
				if (bLookingUp)
					Jump();
				vForce.z = 0.f;
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
	}
	if (m_pTargetObj) {
		Vector3 vTargetPos = m_pTargetObj->Transform()->GetPosition();
		Vector3 vMyPos = Transform()->GetPosition();
		vTargetPos.z = 0.f;
		vMyPos.z = 0.f;
		float fDistance = CMyMath::GetDistance(vTargetPos, vMyPos);
		if (fDistance < m_fMaxDetectDistance)
			ChangeAIState(E_AIState_bu::Notice);
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
float fMaxStunTime = 0.8f;
void CEnemyController_bu::StunStateInit()
{
	m_pEmoticonObj->SetActive(true);
	m_pEmoticonAnim->Play(BUTCHER_AnimName_EmoticonStun, E_AnimationState::Loop, true);
	
	fStunTime = 0.f;
	fMaxStunTime = 0.8f;

	m_pLegAnim->Play(_T("Enemy_Idle_bu"));
}

void CEnemyController_bu::StunStateUpdate()
{
	fStunTime += DT;
	if (fStunTime > fMaxStunTime) {
		ChangeAIState(E_AIState_bu::Notice);
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

float fDetectRange = 15.f;
float fShootableRange = 8.f;
bool isKeepGoToTargetPos = false;
Vector2 vNextPath;
float fFireDelay = 0.f;
float fMaxFireDelay = 1.3f;
bool isFireWait = false;
float fFireWait = 0.f;
float fMaxFireWait = 1.3;
void CEnemyController_bu::FollowStateInit()
{
	fDetectRange = 15.f;
	m_fTargetFindTime = m_fMaxTargetFindTime;
	isKeepGoToTargetPos = false;
	m_pTargetLookAt->SetTargetObj(m_pTargetObj);

	// targetObject 세팅
	// target look at의 타겟을 세팅

	isFireWait = false;
	fFireDelay = 0.f;
	fFireWait = 0.f;
	fMaxFireWait = (rand() % 3 + 10) * 0.1f;
}

void CEnemyController_bu::FollowStateUpdate()
{
	if (!m_pTargetObj) {
		ChangeAIState(E_AIState_bu::Wander);
		return;
	}
		
	Vector3 vTargetPos = m_pTargetObj->Transform()->GetPosition();
	Vector3 vMyPos = Transform()->GetPosition();

	float fLength = Vector2::Distance(vTargetPos.XY(), vMyPos.XY());

	bool bKeepFollow = true;
	if (fLength > fDetectRange)
		bKeepFollow = false;

	if (!bKeepFollow)
		ChangeAIState(E_AIState_bu::Bewildered);
	else {
		bool isPathFound = false;
		m_fTargetFindTime += DT;

		// step1 : 경로를 찾는다.
		if (m_fTargetFindTime > m_fMaxTargetFindTime) {
			isPathFound = m_pPathFind->FindPath(vMyPos.XY(), vTargetPos.XY());
			m_fTargetFindTime = 0.f;
		}

		// 다음 경로가 존재한다면 
		list<Vector2>& listNextPath = m_pPathFind->GetPath();
		if (!listNextPath.empty()) {
			// 다음 경로의 위치를 구한다.
			Vector2 vNextPath = m_pPathFind->GetNextPath();
			Vector3 vNextPos = Vector3(vNextPath.x, vNextPath.y, vMyPos.z);

			Vector3 vMoveDir = vNextPos - vMyPos;
			vMoveDir.Normalize();

			// 무기에 따라서 움직일지 안움직일지 결정한다.
			bool go = false;
			if (E_WeaponType_bu::Chainsaw == GetWeapon()->GetCurWeaponType()) {
				go = true;
			}
			else {
				float fDis = Vector3::Distance(vTargetPos, vMyPos);
				if (fDis < fShootableRange) 
					go = false;
				else 
					go = true;
			}

			if (go) {
				Vector3 vForce = vMoveDir * m_fMovePower;
				float fDegree = CMyMath::VectorToDegree(vMoveDir.XY());
				bool bLookingUp = fDegree >= 45.f && fDegree <= 135.f;
				if (bLookingUp)
					Jump();

				vForce.y = 0.f;
				Vector3 vForceDir = Vector3(vMoveDir.x, 0.f, 0.f);
				vForceDir.Normalize();
				Rigidbody2D()->AddForce(vForceDir * m_fMovePower);
				ChangeState(E_CharacterState::Move);
			}
			else {
				ChangeState(E_CharacterState::Idle);
			}


			// 이미지 방향을 바꾼다.
			if (vMoveDir.x > 0)
				_SetLookRightState(true);
			else
				_SetLookRightState(false);
			float vDis = Vector2::Distance(vNextPath, Transform()->GetPosition().XY());
			if (vDis < 0.7f) {
				listNextPath.pop_front();
			}
		}
	}


	if (GetWeapon()->GetCurWeaponType() == E_WeaponType_bu::MachineGun) {
		if (!isFireWait) {
			fFireDelay += DT;
			if (fFireDelay > fMaxFireDelay) {
				fFireDelay = 0.f;
				isFireWait = true;
			}
			else {
				Vector3 vmuzzlePos = m_pMuzzleObj->Transform()->GetPosition();
				Vector3 vrotPos = m_pGunRotationPosObj->Transform()->GetRotation();
				Vector3 vfrontVec = m_pGunRotationPosObj->Transform()->GetRightVector();
				UINT iLayer = (UINT)E_Tag::Enemy_Bullet;
				m_pWeapon->Fire(vmuzzlePos, vrotPos, vfrontVec, iLayer);
			}
		}
		else {
			fFireWait += DT;
			if (fFireWait > fMaxFireWait) {
				fFireWait = 0.f;
				isFireWait = false;
			}
		}
	}
	else {
		Vector3 vmuzzlePos = m_pMuzzleObj->Transform()->GetPosition();
		Vector3 vrotPos = m_pGunRotationPosObj->Transform()->GetRotation();
		Vector3 vfrontVec = m_pGunRotationPosObj->Transform()->GetRightVector();
		UINT iLayer = (UINT)E_Tag::Enemy_Bullet;
		m_pWeapon->Fire(vmuzzlePos, vrotPos, vfrontVec, iLayer);
	}
}

void CEnemyController_bu::FollowStateEnd()
{
	m_pTargetLookAt->SetTargetObj(nullptr);
}

float fKeepNoticeTime = 0.f;
float fMaxKeepNoticeTime = 1.5f;
void CEnemyController_bu::NoticeStateInit()
{
	m_pEmoticonObj->SetActive(true);
	m_pEmoticonAnim->Play(BUTCHER_AnimName_EmoticonSuprise, E_AnimationState::Loop, true);
	ChangeState(E_CharacterState::Idle);

	fKeepNoticeTime = 0.f;
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
	ChangeState(E_CharacterState::Idle);
}

void CEnemyController_bu::BewilderedStateUpdate()
{
	fBewilderedTime += DT;
	if(fBewilderedTime > fBewilderedMaxTime)
		ChangeAIState(E_AIState_bu::Wander);
}

void CEnemyController_bu::BewilderedStateEnd()
{
	m_pEmoticonObj->SetActive(false);
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

void CEnemyController_bu::OnMoveStart()
{
	// Animation 추가
	m_pLegAnim->Play(_T("Enemy_Walk_bu"), E_AnimationState::Loop);
}

void CEnemyController_bu::OnMoveUpdate()
{
	if (m_pSoundMgr) {
		m_fFootstepSoundDelTime += DT;
		if (m_fFootstepSoundDelTime > m_fMaxFootstepSoundDelTime) {
			int iRand = rand() % 3;
			if (iRand == 0)
				m_pSoundMgr->m_pFootStep1->Play(1, 0.5f, true);
			else if (iRand == 1)
				m_pSoundMgr->m_pFootStep2->Play(1, 0.5f, true);
			else if (iRand == 2)
				m_pSoundMgr->m_pFootStep3->Play(1, 0.5f, true);
			m_fFootstepSoundDelTime = 0.f;
		}
	}
}

void CEnemyController_bu::OnMoveEnd()
{
}

void CEnemyController_bu::OnJumpStart()
{
	m_pLegAnim->Play(_T("Enemy_Jump_bu"), E_AnimationState::Loop);
}

void CEnemyController_bu::OnJumpUpdate()
{
}

void CEnemyController_bu::OnJumpEnd()
{
}

void CEnemyController_bu::OnDeadStart()
{
	UINT iLayer = (UINT)E_Layer::ObjectParticle;
	int size = m_pBodyPartPref->GetProtoObj()->Animator2D()->GetCurAnimation()->GetAnimationFrame().size();
	
	if (m_vBodySplitDir == Vector3::Zero) {
		float degree = 180.f / size;
		for (int i = 0; i < size; ++i) {
			Vector3 vDir = ::Rotate(Vector3::Right, i * degree);
			CGameObject* pObj = CObject::InstantiateEvn(m_pBodyPartPref, Transform()->GetPosition(), iLayer);
			pObj->Animator2D()->SetCurAnimationFrame(i);
			CExplosion_bu* pExp = pObj->GetComponent<CExplosion_bu>();
			if (pExp)
				pExp->SetExplosion(vDir, 2.0f, 0.3f);
		}
	}
	else {
		float degree = 90.f / size;
		for (int i = 0; i < size; ++i) {
			Vector3 vDir = ::Rotate(m_vBodySplitDir, 45.f - i * degree);
			CGameObject* pObj = CObject::InstantiateEvn(m_pBodyPartPref, Transform()->GetPosition(), iLayer);
			pObj->Animator2D()->SetCurAnimationFrame(i);
			CExplosion_bu* pExp = pObj->GetComponent<CExplosion_bu>();
			if (pExp)
				pExp->SetExplosion(vDir, 2.0f, 0.15f);
		}
	}
	
	if (m_pSoundMgr) {
		int iRandSound = rand() % 3;
		if (iRandSound == 0)
			m_pSoundMgr->m_pScream1->Play(1, 1, true);
		else if (iRandSound == 1)
			m_pSoundMgr->m_pScream2->Play(1, 1, true);
		else if (iRandSound == 2)
			m_pSoundMgr->m_pScream3->Play(1, 1, true);
	}

	DestroyGameObjectEvn(GetGameObject()); 
}

void CEnemyController_bu::OnDeadUpdate()
{
}

void CEnemyController_bu::OnDeadEnd()
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
