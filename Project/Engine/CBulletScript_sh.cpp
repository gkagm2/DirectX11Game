#include "pch.h"
#include "CBulletScript_sh.h"

CBulletScript_sh::CBulletScript_sh() :
	m_fSpeed(100.f),
	m_vDir(Vector3(0.f,-1.f,0.f)),
	m_eType(E_BulletType_sh::Straight)
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
	// Move
	m_vDir.Normalize();
	Vector3 vPos = Transform()->GetLocalPosition();
	Transform()->SetLocalPosition(vPos + m_vDir * m_fSpeed * DeltaTime);
}