#include "pch.h"
#include "CCore.h"

#include "CGameManagerScript_sh.h"

#include "CPlayerScript_sh.h"
#include "CBulletScript_sh.h"

CPlayerScript_sh::CPlayerScript_sh() :
	m_fHp(10.f),
	m_fBulletDamage(4.f),
	m_fMoveSpeed(200.f)
{
}

CPlayerScript_sh::~CPlayerScript_sh()
{
}

void CPlayerScript_sh::Start()
{
	GetGameObject()->SetName(_T("Player"));
}

void CPlayerScript_sh::Update()
{
	CGameManagerScript_sh* pGameManager = (CGameManagerScript_sh*) FIND_GameObject(STR_OBJ_NAME_GameManager);
	if (E_GameState_sh::GameOver == pGameManager->GetGameState())
		return;

	Move();

	if (InputKeyPress(E_Key::SPACE))
		Shoot();
}

void CPlayerScript_sh::Shoot()
{
	CGameObject* pBulletObj = new CGameObject;
	pBulletObj->AddComponent<CTransform>();
	pBulletObj->AddComponent<CMeshRenderer>();
	pBulletObj->AddComponent<CBulletScript_sh>();

	Vector3 vPlayerPos = Transform()->GetLocalPosition();
	Vector3 vPlayerScale = Transform()->GetLocalScale();
	Vector3 vMuzzlePos = vPlayerPos;
	vMuzzlePos.y += vPlayerScale.y * 0.5f;

	pBulletObj->Transform()->SetLocalPosition(vMuzzlePos);
	pBulletObj->Transform()->SetLocalScale(vPlayerScale * 0.3f);

	pBulletObj->MeshRenderer()->SetMesh(CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMash));
	pBulletObj->MeshRenderer()->SetMaterial(CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_StdMtrlAlphaBlend_Coverage));

	CBulletScript_sh* pBullet = pBulletObj->GetComponent<CBulletScript_sh>();
	pBullet->SetDirection(Vector3(0.f, 1.f, 0.f));

 	CreateGameObjectEvn(pBulletObj, E_Layer::Bullet);
}

void CPlayerScript_sh::Move()
{
	Vector3 vDir{0,0,0};

	Vector3 vPosition = Transform()->GetLocalPosition();
	if (InputKeyHold(E_Key::A))
		vDir.x -= 1.f;
	if (InputKeyHold(E_Key::D))
		vDir.x += 1.f;
	if (InputKeyHold(E_Key::W))
		vDir.y += 1.f;
	if (InputKeyHold(E_Key::S))
		vDir.y -= 1.f;

	vDir.Normalize();
	vPosition = vPosition + vDir * m_fMoveSpeed * DeltaTime;
	Transform()->SetLocalPosition(vPosition);
}