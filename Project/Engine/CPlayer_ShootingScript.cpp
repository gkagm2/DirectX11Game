#include "pch.h"
#include "CShootingGameManagerScript.h"

#include "CPlayer_ShootingScript.h"
#include "CBullet_ShootingScript.h"

CPlayer_ShootingScript::CPlayer_ShootingScript() :
	m_fHp(10.f),
	m_fBulletDamage(4.f)
{
}

CPlayer_ShootingScript::~CPlayer_ShootingScript()
{
}

void CPlayer_ShootingScript::Start()
{
}

void CPlayer_ShootingScript::Update()
{
	CGameObject* pGameManager = FIND_GameObject(STR_SHOOTING_OBJ_NAME_ShootingGameManager);
}

void CPlayer_ShootingScript::Shoot()
{
	CGameObject* pBulletObj = new CGameObject;
	pBulletObj->AddComponent<CTransform>();
	pBulletObj->AddComponent<CMeshRenderer>();
	pBulletObj->AddComponent<CBullet_ShootingScript>();

	Vector3 vPlayerPos = Transform()->GetLocalPosition();
	Vector3 vPlayerScale = Transform()->GetLocalScale();
	vPlayerPos.y += vPlayerScale.y * 0.5f;

	pBulletObj->MeshRenderer()->SetMesh(CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMash));
	pBulletObj->MeshRenderer()->SetMaterial(CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_StandardMaterialAlphaBlend_Coverage));

	CSceneManager::GetInstance()->GetCurScene()->AddGameObject(pBulletObj,E_Layer::Bullet);
}

void CPlayer_ShootingScript::Move()
{
	// TODO (Jang) : 움직임 만들기
	if (InputKeyHold(E_Key::A)) {

	}
	if (InputKeyHold(E_Key::D)) {

	}
	if (InputKeyHold(E_Key::W)) {

	}
	if (InputKeyHold(E_Key::S)) {

	}
}