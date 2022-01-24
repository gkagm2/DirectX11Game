#include "pch.h"
#include "CChainsaw_bu.h"
#include "CCharacter_bu.h"

CChainsaw_bu::CChainsaw_bu() :
	CScript((UINT)SCRIPT_TYPE::CHAINSAW_BU),
	m_fDamaged(5.f)
{
}

void CChainsaw_bu::OnCollisionEnter2D(CCollider2D* _pCol)
{
	CGameObject* pObj = _pCol->GetGameObject();
	bool isObjLayer = pObj->GetLayer() == (UINT)E_Layer::Object;
	bool isEnemyTag = pObj->GetTag() == (UINT)E_Tag::Enemy;
	bool isPlayerTag = pObj->GetTag() == (UINT)E_Tag::Player;
	
	if (isObjLayer) {
		if ((UINT)GetGameObject()->GetTag() == (UINT)E_Tag::Enemy_Bullet && isPlayerTag) {
			CCharacter_bu* pChar = pObj->GetComponent<CCharacter_bu>();
			pChar->SetBodySplitDir(Vector3::Zero);
			pChar->DamagedMe(m_fDamaged);
		}
		if ((UINT)GetGameObject()->GetTag() == (UINT)E_Tag::Player_Bullet && isEnemyTag) {
			CCharacter_bu* pChar = pObj->GetComponent<CCharacter_bu>();
			if(rand()%2 == 0)
				pChar->SetBodySplitDir(Vector3::Zero);
			else
				pChar->SetBodySplitDir(Transform()->GetRightVector());
			pChar->DamagedMe(m_fDamaged);
		}
	}
}