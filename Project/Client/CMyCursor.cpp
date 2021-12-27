//#include "pch.h"
//#include "CMyCursor.h"
//#include <Engine\CObjectManager.h>
//#include <Engine\CSceneManager.h>
//#include <Engine\CScene.h>
//#include <Engine\CGameObject.h>
//#include <Script\CCursorScript.h>
//
//CGameObject* CMyCursor::m_pObj = nullptr;
//
//void CMyCursor::SetCursor(SharedPtr<CTexture> _pTex, const Vector2& vSize)
//{
//	CGameObject* pObj = FIND_GameObject_Layer(STR_OBJ_NAME_Cursor, NUM_LAYER_UI);
//	if (!pObj) {
//		pObj = CObjectManager::GetInstance()->CreateImageUI();
//		pObj->AddComponent<CCursorScript>();
//		m_pObj = pObj;
//	}
//	CCursorScript* pcursor =  m_pObj->GetComponent<CCursorScript>();
//	pcursor->SetCurCursor(_pTex, vSize);
//}