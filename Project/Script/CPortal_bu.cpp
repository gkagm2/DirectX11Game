#include "pch.h"
#include "CPortal_bu.h"
#include "CPlayerController_bu.h"
#include "CGameManager_bu.h"

CPortal_bu::CPortal_bu() :
	CScript((UINT)SCRIPT_TYPE::PORTAL_BU)
{
}

void CPortal_bu::Awake()
{
	//CPlayerController_bu* pPlayer = nullptr;
	//FIND_Component(pPlayer, CPlayerController_bu);
	//assert(pPlayer);
	//if (pPlayer) {
	//	m_pPlayerObj = pPlayer->GetGameObject();
	//	assert(m_pPlayerObj);
	//}

	Vector3 vPortalPos = Transform()->GetPosition();
	if (m_pPlayerObj) {
		m_vOrignZ = m_pPlayerObj->Transform()->GetLocalPosition().z;
		vPortalPos.z = 100.f; // 안보이게 뒤로 보냄
		m_pPlayerObj->Transform()->SetLocalPosition(vPortalPos);
	}
	assert(Animator2D());
	Animator2D()->Play(E_AnimationState::Once, true);

	m_pLight = GetGameObject()->FindComponentInChilds<CLight2D>();
	assert(m_pLight);
}

void CPortal_bu::Update()
{
	static CGameManager_bu* pGameMgr = FIND_GameObject(_T("GameManager"))->GetComponent<CGameManager_bu>();
	if (pGameMgr->GetGameMode() != E_GameMode_bu::Play)
		return;

	/*if (!m_pPlayerObj || m_pPlayerObj->IsDead()) {
		m_pPlayerObj = nullptr;
		return;
	}*/

	CAnimation2D* pCurAnim = Animator2D()->GetCurAnimation();
	int iCurIdx = pCurAnim->GetCurFrameIdx();
	if (pCurAnim->IsFinished()) {
		//Vector3 vPlayerPos = m_pPlayerObj->Transform()->GetLocalPosition();
		//vPlayerPos.z = m_vOrignZ;
		//m_pPlayerObj->Transform()->SetLocalPosition(vPlayerPos);
		DestroyGameObjectEvn(GetGameObject());
	}
	float fR = (rand() % 255) / 255.f;
	float fG = (rand() % 255) / 255.f;
	float fB = (rand() % 255) / 255.f;
	m_pLight->SetDiffColor(Vector3(fR, fG, fB));
	float fRange = (rand() % 4) / 10.f + 0.5f;
	m_pLight->SetRange(fRange);
}