#include "pch.h"
#include "CBulletScript_sh.h"
#include "CGameManagerScript_sh.h"

CBulletScript_sh::CBulletScript_sh() :
	CScript((UINT)SCRIPT_TYPE::BULLETSCRIPT_SH),
	m_fSpeed(400.f),
	m_vDir(Vector3(0.f,-1.f,0.f)),
	m_eType(E_BulletType_sh::Straight),
	m_fDeleteMaxTime(4.f),
	m_fDeleteCoolTime(0.f)
{
}

CBulletScript_sh::~CBulletScript_sh()
{
}

void CBulletScript_sh::Start()
{
 	GetGameObject()->SetName(STR_OBj_NAME_Bullet);
}

void CBulletScript_sh::Update()
{
	/*CGameManagerScript_sh* pGameMgr = (CGameManagerScript_sh*)FIND_GameObject(STR_OBJ_NAME_GameManager)->GetComponent<CScript>();
	if (E_GameState_sh::GameOver == pGameMgr->GetGameState())
		return;*/

	m_fDeleteCoolTime += DT;
	if (m_fDeleteCoolTime > m_fDeleteMaxTime)
		DestroyGameObjectEvn(GetGameObject());

	// Move
	m_vDir.Normalize();
	Vector3 vPos = Transform()->GetLocalPosition();
	Transform()->SetLocalPosition(vPos + m_vDir * m_fSpeed * DT);
}