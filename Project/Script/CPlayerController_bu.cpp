#include "pch.h"
#include "CPlayerController_bu.h"
#include "CBullet_bu.h"
#include <Engine\CCore.h>
#include "CInteractiveObj_bu.h"
CPlayerController_bu::CPlayerController_bu() :
	CCharacter_bu((UINT)SCRIPT_TYPE::PLAYERCONTROLLER_BU),
	m_pRigid(nullptr),
	m_pAnim(nullptr),
	m_pBulletPref(nullptr),
	m_fShotTime(0.3f),
	m_fMaxShotTime(0.3f)
{
	AddParam(TScriptParam(_T("Bullet Prefab"), E_ScriptParam::PREFAB, m_pBulletPref.GetAddress()));
}

CPlayerController_bu::~CPlayerController_bu()
{
}

void CPlayerController_bu::Awake()
{
	CCharacter_bu::Awake();
	m_pMuzzleObj = GetGameObject()->FindGameObjectInChilds(BUTCHER_ObjName_Muzzle);
	assert(m_pMuzzleObj);
	m_pGunRotationPosObj = GetGameObject()->FindGameObjectInChilds(BUTCHER_ObjName_RotationPos);
	assert(m_pGunRotationPosObj);
}

void CPlayerController_bu::Start()
{
	m_pRigid = GetGameObject()->GetComponent<CRigidbody2D>();
	assert(m_pRigid); 
	/*m_pAnim = GetGameObject()->GetComponent<CAnimator2D>();
	assert(m_pAnim);*/
}

void CPlayerController_bu::Update()
{
	if (InputKeyHold(E_Key::A)) { // left
		m_vCurMoveDir = Vector3(-1.f, 0.f, 0.f);
		Move();
	}
	if (InputKeyHold(E_Key::D)) { // right
		m_vCurMoveDir = Vector3(1.f, 0.f, 0.f);
		Move();
	}
	
	if (InputKeyHold(E_Key::W)) {
		Jump();
	}

	m_fShotTime += DT;
	if (InputKeyHold(E_Key::LBUTTON)) {
		if (m_fShotTime >= m_fMaxShotTime) {
			Attack();
			m_fShotTime = 0.f;
		}
	}

	// number pad 1번부터 시작해서 총 갈기
	for (int i = 0; i < (UINT)E_WeaponType_bu::End; ++i) {
		UINT iNum = (UINT)E_Key::NUM1 + i;
		if (InputKeyPress((E_Key)iNum))
			ChangeWeapon((E_WeaponType_bu)i);
	}
}

void CPlayerController_bu::OnCollisionStay2D(CCollider2D* pCol)
{
	CGameObject* pObj = pCol->GetGameObject();
	CInteractiveObj_bu* pInteractive = pObj->GetComponent< CInteractiveObj_bu>();

	// 상호작용하는 오브젝트면
	if (pInteractive) {
		// Space bar를 누르면 오브젝트 작동함.
		if (InputKeyPress(E_Key::SPACE)) {
			bool bActive = pInteractive->IsActive();
			pInteractive->Interaction(!bActive);
		}
	}
}

void CPlayerController_bu::Attack()
{
#ifdef _BUTCHER_GAME
	UINT iLayer = (UINT)E_Layer::Object;
#elif
	UINT iLayer = 1;
#endif
	CGameObject* pBulletObj = m_pBulletPref->Instantiate();
	CBullet_bu* pBul = pBulletObj->GetComponent<CBullet_bu>();

	Vector3 vMuzzlePos = m_pMuzzleObj->Transform()->GetPosition();
	Vector3 vShootDir = m_pGunRotationPosObj->Transform()->GetRightVector();
	Vector3 vRot = m_pGunRotationPosObj->Transform()->GetRotation();

	pBul->Transform()->SetLocalPosition(vMuzzlePos);
	pBul->SetShootDir(vShootDir);
	pBul->Transform()->SetLocalRotation(vRot);

	CObject::CreateGameObjectEvn(pBulletObj, iLayer);
}

void CPlayerController_bu::Jump()
{
	Vector3 vJumpPower = Vector3(0.f, 1.f, 0.f);
	vJumpPower *= m_fJumpPower;
	m_pRigid->AddForce(vJumpPower);
}

void CPlayerController_bu::Move()
{
	Vector3 vMovePower = m_vCurMoveDir;
	vMovePower *= m_fMovePower;
	m_pRigid->AddForce(vMovePower);
}


void CPlayerController_bu::OnDead()
{
	// Dead Animation Start
	//m_pAnim->Play(BUTCHER_Anim_Player_Dead, E_AnimationState::Once);
}

bool CPlayerController_bu::SaveToScene(FILE* _pFile)
{
	SaveResourceToFile(m_pBulletPref, _pFile);
	return true;
}

bool CPlayerController_bu::LoadFromScene(FILE* _pFile)
{
	LoadResourceFromFile(m_pBulletPref, _pFile);
	return true;
}
