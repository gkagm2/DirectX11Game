#include "pch.h"
#include "CSpotLight_bu.h"

CSpotLight_bu::CSpotLight_bu() :
	CScript((UINT)SCRIPT_TYPE::SPOTLIGHT_BU)
{
}

void CSpotLight_bu::OnCollisionEnter2D(CCollider2D* _pCol)
{
	bool isObjLayer= _pCol->GetGameObject()->GetLayer() == (UINT)E_Layer::Object;
	bool isPlayerBullet = _pCol->GetGameObject()->GetTag() == (UINT)E_Tag::Player_Bullet;
	bool isEnemyBullet = _pCol->GetGameObject()->GetTag() == (UINT)E_Tag::Enemy_Bullet;

	if (isObjLayer) {
		if (isPlayerBullet || isEnemyBullet)
			DestroyGameObjectEvn(GetGameObject());
	}
}

void CSpotLight_bu::Update()
{
	if (InputKeyPress(E_Key::SPACE)) {
		DestroyGameObjectEvn(GetGameObject());
	}
}

void CSpotLight_bu::OnDestroy()
{
	if (nullptr != m_pSparkParticle)
		InstantiateEvn(m_pSparkParticle, Transform()->GetPosition(), (UINT)E_Layer::ObjectParticle);
}