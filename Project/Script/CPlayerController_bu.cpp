#include "pch.h"
#include "CPlayerController_bu.h"
#include <Engine\CCore.h>
#include "CInteractiveObj_bu.h"
#include "CCameraFollowerScript.h"

#include "CUIManager_bu.h"
#include "CUIContainer_bu.h"
#include "CInGamePanel_bu.h"
CPlayerController_bu::CPlayerController_bu() :
	CCharacter_bu((UINT)SCRIPT_TYPE::PLAYERCONTROLLER_BU),
	m_pRigid(nullptr),
	m_pLegAnim(nullptr),
	m_pTorsoAnimSprite(nullptr),
	m_pBulletPref(nullptr),
	m_pUIManager(nullptr)
{
}

CPlayerController_bu::~CPlayerController_bu()
{

}

void CPlayerController_bu::Awake()
{
	CCharacter_bu::Awake();
	
	CGameObject* pLegsObj = GetGameObject()->FindGameObjectInChilds(BUTCHER_ObjName_Legs);
	CGameObject* pTorsoObj = GetGameObject()->FindGameObjectInChilds(BUTCHER_ObjName_Torse);
	m_pUIManager = FIND_GameObject(_T("UIManager"))->GetComponent<CUIManager_bu>();
	assert(m_pUIManager);
	assert(pLegsObj);
	assert(pTorsoObj);
	if(pLegsObj)
		m_pLegAnim = pLegsObj->Animator2D();
	if(pTorsoObj)
		m_pTorsoAnimSprite = pTorsoObj->Animator2D();

	assert(m_pLegAnim);
	assert(m_pTorsoAnimSprite);
}

void CPlayerController_bu::Start()
{
	m_pRigid = GetGameObject()->GetComponent<CRigidbody2D>();
	assert(m_pRigid); 

	if (m_CurStateStartFunc)
		m_CurStateStartFunc();
	m_pWeapon->SetUseableWeapon(E_WeaponType_bu::Chainsaw, true);

	// Test Code
	for (UINT i = 0; i < (UINT)E_WeaponType_bu::End; ++i) 
		m_pWeapon->SetUseableWeapon((E_WeaponType_bu)i, true);
}

void CPlayerController_bu::Update()
{
	OnBehavior();
	if (m_CurStateUpdateFunc)
		m_CurStateUpdateFunc();
}

void CPlayerController_bu::OnCollisionStay2D(CCollider2D* pCol)
{
	CGameObject* pObj = pCol->GetGameObject();
	UINT iColTag = pObj->GetTag();
	CInteractiveObj_bu* pInteractive = pObj->GetComponent< CInteractiveObj_bu>();

	// ��ȣ�ۿ��ϴ� ������Ʈ��
	if (pInteractive) {
		// Space bar�� ������ ������Ʈ �۵���.
		if (InputKeyPress(E_Key::SPACE)) {
			bool bActive = pInteractive->IsActive();
			pInteractive->Interaction(!bActive);
		}
	}
	//
	//UINT iTag = (UINT)E_Tag::Bullet;
	//if ((E_Tag)iTag == (E_Tag)iColTag) {
	//	CBullet_bu* pBullet = pObj->GetComponent<CBullet_bu>();
	//}

	//iTag = (UINT)E_Tag::DangerObj;
	//if ((E_Tag)iTag == (E_Tag)iColTag) {

	//}
}

void CPlayerController_bu::OnBehavior()
{
	bool isMove = false;
	bool isAttack = false;

	bool isWeaponSwap = false;
	int iWeaponIdx = 0;

	if (InputKeyHold(E_Key::A)) { // left
		m_vCurMoveDir = Vector3(-1.f, 0.f, 0.f);
		isMove = true;
	}
	if (InputKeyHold(E_Key::D)) { // right
		m_vCurMoveDir = Vector3(1.f, 0.f, 0.f);
		isMove = true;
	}

	if (InputKeyPress(E_Key::W)) {
		if (m_bCanJump) {
			m_bJump = true;
			ChangeState(E_CharacterState::Jump);
			m_fJumpCoolTime = 0.f;
		}

	}

	if (InputKeyHold(E_Key::LBUTTON)) {
		isAttack = true;
	}

	// Weapon Swap
	for (int i = 0; i < (UINT)E_WeaponType_bu::End; ++i) {
		UINT iNum = (UINT)E_Key::NUM1 + i;
		if (InputKeyPress((E_Key)iNum)) {
			isWeaponSwap = true;
			iWeaponIdx = i;
		}
	}

	if (isMove) {
		Vector3 vMovePower = m_vCurMoveDir;
		vMovePower *= m_fMovePower;
		m_pRigid->AddForce(vMovePower);
		ChangeState(E_CharacterState::Move);
	}
	else {
		ChangeState(E_CharacterState::Idle);
	}

	if (m_bJump) {
		m_fJumpCoolTime += DT;
		if (m_fJumpCoolTime <= m_fJumpMaxCoolTime) {
			{
				Vector3 vJumpPower = Vector3(0.f, 1.f, 0.f);
				vJumpPower *= m_fJumpPower;
				Vector3 vPower = (m_fJumpMaxCoolTime - m_fJumpCoolTime)* vJumpPower* 2;
				m_pRigid->AddForce(vPower);
			}
		}
		else
			m_bJump = false;
	}

	if (isAttack) {
		Vector3 vmuzzlePos = m_pMuzzleObj->Transform()->GetPosition();
		Vector3 vrotPos = m_pGunRotationPosObj->Transform()->GetRotation();
		Vector3 vfrontVec = m_pGunRotationPosObj->Transform()->GetRightVector();
		bool isFired = m_pWeapon->Fire(vmuzzlePos, vrotPos, vfrontVec, (UINT)E_Tag::Player_Bullet);
		if (isFired) {
			// cam shaking
			CCamera* pMainCam = CRenderManager::GetInstance()->GetMainCamera();
			if (pMainCam) {
				CCameraFollowerScript* pCamFol =pMainCam->GetGameObject()->GetComponent<CCameraFollowerScript>();
				pCamFol->Shaking(false, 0.1f, 0.05f, -vfrontVec);
			}
		}
	}

	if (isWeaponSwap)
		ChangeWeapon((E_WeaponType_bu)iWeaponIdx);


	// ������ ���� ��ü �ִϸ��̼��� �ٸ��� �Ѵ�.
	CAnimation2D* pAnim = m_pTorsoAnimSprite->GetCurAnimation();
	if (pAnim) {
		UINT iSize = (UINT)pAnim->GetAnimationFrame().size();
		// Rotation angle�� ����.
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
	float fDegree = m_pGunRotationPosObj->Transform()->GetRotationDegree().z;
	if (fDegree <= 90.f && fDegree >= -90.f)
		m_bIsLookRight = true;
	else
		m_bIsLookRight = false;

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
}

void CPlayerController_bu::OnMoveStart()
{
	// Animation �߰�
	m_pLegAnim->Play(_T("Player_Walk_bu"), E_AnimationState::Loop);
}

void CPlayerController_bu::OnMoveUpdate()
{
}

void CPlayerController_bu::OnMoveEnd()
{
}

void CPlayerController_bu::OnIdleStart()
{
	m_pLegAnim->Play(_T("Player_Idle_bu"), E_AnimationState::Loop);
}

void CPlayerController_bu::OnIdleUpdate()
{
}

void CPlayerController_bu::OnIdleEnd()
{
}

void CPlayerController_bu::OnJumpStart()
{
	m_pLegAnim->Play(_T("Player_Jump_bu"), E_AnimationState::Loop);
}

void CPlayerController_bu::OnJumpUpdate()
{
	
}

void CPlayerController_bu::OnJumpEnd()
{
}

void CPlayerController_bu::OnDeadStart()
{
}

void CPlayerController_bu::OnDeadUpdate()
{
}

void CPlayerController_bu::OnDeadEnd()
{
}

void CPlayerController_bu::UpdateUI()
{
}

bool CPlayerController_bu::SaveToScene(FILE* _pFile)
{
	return true;
}

bool CPlayerController_bu::LoadFromScene(FILE* _pFile)
{
	return true;
}
