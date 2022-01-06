#include "pch.h"
#include "CItem_bu.h"
#include "CPlayerController_bu.h"

CItem_bu::CItem_bu() :
	CScript((UINT)SCRIPT_TYPE::ITEM_BU),
	m_eItemType(E_ItemType_bu::Hp),
	m_bIsLifeTimeOn(false),
	m_fMaxLifeTime(10.f),
	m_fLifeTime(0.f)
{
}

CItem_bu::~CItem_bu()
{
}

void CItem_bu::Start()
{
}

void CItem_bu::Update()
{
	if (m_bIsLifeTimeOn) {
		m_fLifeTime += DT;

		if (m_fLifeTime > m_fMaxLifeTime)
			CObject::DestroyGameObjectEvn(GetGameObject());
	}
}

bool CItem_bu::SaveToScene(FILE* _pFile)
{
	FWrite(m_eItemType, _pFile);
	return true;
}

bool CItem_bu::LoadFromScene(FILE* _pFile)
{
	FRead(m_eItemType, _pFile);
	return true;
}

void CItem_bu::OnCollisionEnter2D(CCollider2D* _pOther)
{
#ifdef _BUTCHER_GAME
	UINT playerTag = (UINT)E_Tag::Player;
#elif
	UINT playerTag = 1;
#endif

	if (playerTag == _pOther->GetGameObject()->GetTag()) {
		CPlayerController_bu* pctr = _pOther->GetGameObject()->GetComponent<CPlayerController_bu>();
	}
}

void CItem_bu::GainItem()
{

}