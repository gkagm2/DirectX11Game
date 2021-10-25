#include "pch.h"
#include "CGameManagerScript_sh.h"
#include "CEnemyScript_sh.h"
#include <Engine\CCore.h>
#include <Engine\CLight2D.h>

CEnemyScript_sh::CEnemyScript_sh() :
	CScript((UINT)SCRIPT_TYPE::ENEMYSCRIPT_SH),
	m_fHp(10.f),
	m_fMoveSpeed(30.f),
	m_fBulletSpeed(100.f),
	m_iColorFlag(0)
{
	m_fMoveSpeed = (float)(rand() % 90 + 50);
}

CEnemyScript_sh::~CEnemyScript_sh()
{
}

void CEnemyScript_sh::Awake()
{
}

void CEnemyScript_sh::Start()
{
	GetGameObject()->SetName(STR_OBJ_NAME_Enemy);
	m_pSharedMtrl = MeshRenderer()->GetSharedMaterial();
	m_pCloneMtrl = MeshRenderer()->GetCloneMaterial();
}

void CEnemyScript_sh::Update()
{
	CGameManagerScript_sh* pGameMgr = FIND_GameObject(STR_OBJ_NAME_GameManager)->GetComponent<CGameManagerScript_sh>();
	if (E_GameState_sh::GameOver == pGameMgr->GetGameState())
		return;

	Vector2 vResolution = CCore::GetInstance()->GetWindowResolution();

	Vector3 vCurPos = Transform()->GetLocalPosition();
	if (vCurPos.y < -(vResolution.y * 0.8f)) {
 		pGameMgr->SetGameState(E_GameState_sh::GameOver);
		MessageBox(nullptr, _T("GameOver"), _T("GameOver"), MB_OK);
		DestroyGameObjectEvn(GetGameObject());
		return;
	}


	Move();
}

void CEnemyScript_sh::Move()
{
	Vector3 vMovePos = Transform()->GetLocalPosition();
	vMovePos.y -= m_fMoveSpeed * DT;
	Transform()->SetLocalPosition(vMovePos);
}

void CEnemyScript_sh::ColorChange()
{
	if (m_iColorFlag == 1) {
		m_iColorFlag = 0;
		MeshRenderer()->SetMaterial(m_pSharedMtrl);
	}
	else {
		m_iColorFlag = 1;
		m_pCloneMtrl->SetData(E_ShaderParam::Int_0, &m_iColorFlag);
		MeshRenderer()->SetMaterial(m_pCloneMtrl);
	}
}

bool CEnemyScript_sh::SaveToScene(FILE* _pFile)
{
	return true;
}

bool CEnemyScript_sh::LoadFromScene(FILE* _pFile)
{
	return true;
}
