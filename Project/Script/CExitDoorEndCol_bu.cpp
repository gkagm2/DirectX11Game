#include "pch.h"
#include "CExitDoorEndCol_bu.h"
#include "CGameManager_bu.h"
#include "CUIManager_bu.h"

CExitDoorEndCol_bu::CExitDoorEndCol_bu() :
	CScript((UINT)SCRIPT_TYPE::EXITDOORENDCOL_BU)
{
}

void CExitDoorEndCol_bu::OnCollisionEnter2D(CCollider2D* pCol)
{
	UINT iObjLayer = (UINT)E_Layer::Object;
	UINT iPlayerTag = (UINT)E_Tag::Player;
	CGameObject* pObj = pCol->GetGameObject();
	if (iObjLayer == pObj->GetLayer() && iPlayerTag == pObj->GetTag()) {
		CGameObject* pGameMgr = FIND_GameObject(_T("GameManager"));
		if (pGameMgr) {
			CGameManager_bu* pmgr = pGameMgr->GetComponent<CGameManager_bu>();
			if (pmgr)
				pmgr->SetGameMode(E_GameMode_bu::SuccessStop);
		}
	}
}