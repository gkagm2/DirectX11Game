#include "pch.h"
#include "CGameManagerScript_sh.h"
#include "CEnemyScript_sh.h"
#include <Engine\CCore.h>
#include <Engine\CLight2D.h>
#include "CPlayerScript_sh.h"
#include "CBulletScript_sh.h"

CEnemyScript_sh::CEnemyScript_sh() :
	CScript((UINT)SCRIPT_TYPE::ENEMYSCRIPT_SH),
	m_fHp(10.f),
	m_fDoorMoveTime(2.f),
	m_fBulletSpeed(3.f),
	m_iColorFlag(0),
	m_vOriginalScale{1.f,1.f,1.f},
	m_fBackDistance(0.3f)
{
	m_fDoorMoveTime = (float)(rand() % 5 + 10);
}

CEnemyScript_sh::~CEnemyScript_sh()
{
}

void CEnemyScript_sh::Awake()
{
}

void CEnemyScript_sh::Start()
{
	m_fHp = 10.f;
	m_fDoorMoveTime = 2.f;
	m_fBulletSpeed = 3.f;
	m_iColorFlag = 0;
	m_vOriginalScale = { 1.f,1.f,1.f };
	m_fDoorMoveTime = (float)(rand() % 5 + 5);

	GetGameObject()->SetName(STR_OBJ_NAME_Enemy);
	m_pSharedMtrl = MeshRenderer()->GetSharedMaterial();
	m_pCloneMtrl = MeshRenderer()->GetCloneMaterial();

	m_vOriginalScale = Transform()->GetScale();
	m_fOriginalHp = m_fHp;
}

void CEnemyScript_sh::Update()
{
	CGameManagerScript_sh* pGameMgr = FIND_GameObject(STR_OBJ_NAME_GameManager)->GetComponent<CGameManagerScript_sh>();
	if (E_GameState_sh::GameOver == pGameMgr->GetGameState())
		return;


	Vector3 vCurPos = Transform()->GetLocalPosition();
	if (vCurPos.y < -10.f) {
 		pGameMgr->SetGameState(E_GameState_sh::GameOver);
		MessageBox(nullptr, _T("GameOver"), _T("GameOver"), MB_OK);
		DestroyGameObjectEvn(GetGameObject());
		return;
	}

	Move();
}

void CEnemyScript_sh::OnCollisionEnter2D(CCollider2D* _pOther)
{
	static int cnt = 0;
	_tcprintf(_T("%d\n"), cnt++);
	UINT iLayer = _pOther->GetGameObject()->GetLayer();
	if (iLayer == 1) {  // bullet
		CBulletScript_sh* pBullet = _pOther->GetGameObject()->GetComponent<CBulletScript_sh>();

		CPlayerScript_sh* pPlayer = pBullet->GetOwnerObject()->GetComponent<CPlayerScript_sh>();
		if (pPlayer) {
			float fDamage = pPlayer->GetBulletDamage();
			SetHp(GetHP() - fDamage);
			float fResultScaleY = (GetHP() / m_fOriginalHp) * m_vOriginalScale.y;
			Vector3 vScale = Transform()->GetScale();
			vScale.y = fResultScaleY;
			Transform()->SetLocalScale(vScale);
			Vector3 vPos = Transform()->GetPosition();
			vPos.y += m_fBackDistance;
			Transform()->SetLocalPosition(vPos);
			float hp = GetHP();
			if (hp <= 0)
				CObject::DestroyGameObjectEvn(GetGameObject());
		}
		DestroyGameObjectEvn(pBullet->GetGameObject());
	}
}

void CEnemyScript_sh::OnCollisionStay2D(CCollider2D* _pOther)
{

}

void CEnemyScript_sh::OnCollisionExit2D(CCollider2D* _pOther)
{

}

void CEnemyScript_sh::Move()
{
	Vector3 vMovePos = Transform()->GetLocalPosition();
	vMovePos.y -= m_fDoorMoveTime * DT;
	Transform()->SetLocalPosition(vMovePos);
}

bool CEnemyScript_sh::SaveToScene(FILE* _pFile)
{
	return true;
}

bool CEnemyScript_sh::LoadFromScene(FILE* _pFile)
{
	return true;
}
