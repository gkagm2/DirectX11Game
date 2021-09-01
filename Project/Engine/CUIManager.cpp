#include "pch.h"

#include "CKeyManager.h"

#include "CUIManager.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CUI.h"
#include "CCamera.h"

CUIManager::CUIManager() :
	m_pCurFocusedUI(nullptr),
	m_pPointDownUI(nullptr)
{
}

CUIManager::~CUIManager() {

}

void CUIManager::Update()
{
	/*
	CScene* pScene = CSceneManager::GetInstance()->GetCurScene();
	if (nullptr == pScene) return;
	vector<CGameObject*> vecUIObjs;
	pScene->GetGameObjects(vecUIObjs, NUM_LAYER_UI);

	// 카메라는 제거
	vector<CGameObject*>::iterator iter = vecUIObjs.begin();
	for (; iter != vecUIObjs.end(); ++iter) {
		if ((*iter)->Camera()) {
			break;
		}
	}
	if (iter != vecUIObjs.end())
		vecUIObjs.erase(iter);

	CUI* pPrevFocusedUI = m_pCurFocusedUI;// 이전에 포커싱된 부모 UI

	// 현재 포커싱된 최상위 부모 UI를 가져옴
	E_KeyState eCurKeyState = CKeyManager::GetInstance()->GetKeyState(E_Key::LBUTTON); // Mouse Left Button 상태를 가져옴

	int iUICnt = (int)vecUIObjs.size();
	if (0 == iUICnt) {
		m_pCurFocusedUI = nullptr;
		m_pPointDownUI = nullptr;
	}

	for (int i = iUICnt - 1; i >= 0; --i) {
		if (((CUI*)vecUIObjs[i])->IsPointerOn(MousePosition) && eCurKeyState == E_KeyState::PRESS) {
			m_pCurFocusedUI = ((CUI*)vecUIObjs[i]);
			break;
		}
	}

	// 포커싱된 UI가 변경되었으면 그룹 벡터에 가장 뒤로 옮김
	if (pPrevFocusedUI != m_pCurFocusedUI) {
		for (auto iter = vecUIObjs.begin(); iter != vecUIObjs.end(); ++iter) {
			if ((*iter) == m_pCurFocusedUI->GetGameObject()) {
				vecUIObjs.erase(iter);
				break;
			}
		}
		vecUIObjs.push_back(m_pCurFocusedUI->GetGameObject());
	}

	// Focus된 최상위 UI가 있다면
	if(m_pCurFocusedUI) {
		// 자식 UI중 타겟팅된 UI를 가져온다. 
		CUI* pTargetUI = m_pCurFocusedUI;

		// 부모로부터 자식 오브젝트 Level Order를 이용하여 찾기 
		queue<CUI*> que;
		que.push(m_pCurFocusedUI);
		while (!que.empty()) {
			CUI* pChildUI = que.front();
			que.pop();

			vector<CUI*>& vecChilds = pChildUI->GetChildsUI();
			for (UINT i = 0; i < vecChilds.size(); ++i)
				que.push(vecChilds[i]);

			if (pChildUI->IsPointerOn(MousePosition)) {
				
				pChildUI->m_bIsOn = true;
				if (pTargetUI->m_bIsOn)
					pTargetUI->m_bIsOn = false;
				pTargetUI = pChildUI;
			}
			else {
				pChildUI->m_bIsOn = false;
			}
		}

		// 마우스 포인터가 UI 외부에 있을 경우
		if (false == pTargetUI->IsPointerOn(MousePosition)) {
			if (pTargetUI->m_bIsOn)
				pTargetUI->m_bIsOn = false;
		}
		else { // 마우스 포인터가 UI 내부에 있을 경우
			if (false == pTargetUI->m_bIsOn)
				pTargetUI->m_bIsOn = true;

			if (InputKeyPress(E_Key::LBUTTON)) {
				m_pPointDownUI = pTargetUI;
				pTargetUI->OnPointerDown();
				pTargetUI->m_bIsDown = true;
			}
		}

		if (InputKeyRelease(E_Key::LBUTTON)) {
			if (nullptr != m_pPointDownUI) {
				m_pPointDownUI->OnPointerUp();

				if (pTargetUI->m_bIsDown && InputKeyRelease(E_Key::LBUTTON)) { //pTargetUI == m_pPointDownUI
					pTargetUI->OnPointerClick();
					pTargetUI->m_bIsDown = false;
				}
				else
					m_pPointDownUI->m_bIsDown = false;
				m_pPointDownUI = nullptr;
			}
		}
	}
	else { // 포커싱된게 아무것도 없으면
		if (InputKeyPress(E_Key::LBUTTON)) { // 왼쪽 마우스 버튼을 눌렀다면
			// 영역내에 들어온 UI가 있는지 체크한다.
			for (UINT i = 0; i < vecUIObjs.size(); ++i) {
				CUI* pParentUI = (CUI*)vecUIObjs[i];
				// 영역에 들어온게 있다면
				if (pParentUI->IsPointerOn(MousePosition)) {
					m_pCurFocusedUI = pParentUI;
					break;
				}
			}
		}
	}
	*/
}

bool CUIManager::IsMousePointInUI()
{
	/*
	vector<CGameObject*> vecUIObjs;
	CSceneManager::GetInstance()->GetCurScene()->GetGameObjects(vecUIObjs, NUM_LAYER_UI);
	queue<CUI*> que;
	for (UINT i = 0; i < vecUIObjs.size(); ++i)
		que.push((CUI*)vecUIObjs[i]);
	
	while (!que.empty()) {
		CUI* pUI = que.front();
		que.pop();
		if (pUI->IsPointerOn(MousePosition))
			return true;
		
		for (UINT i = 0; i < pUI->GetChildsUI().size(); ++i)
			que.push(pUI->GetChildsUI()[i]);
	}
	*/
	return false;
}
