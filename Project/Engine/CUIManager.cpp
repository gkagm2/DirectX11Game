#include "pch.h"

#include "CKeyManager.h"

#include "CUIManager.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CUI.h"
#include "CCamera.h"
#include "CCollisionManager.h"

//Test

#include "CTimeManager.h"

CUIManager::CUIManager() :
	m_pCurFocusedUI(nullptr),
	m_pPointDownUI(nullptr)
{
}

CUIManager::~CUIManager() {

}

void CUIManager::Update()
{
	// 현재 씬을 가져온다
	CScene* pScene = CSceneManager::GetInstance()->GetCurScene();
	if (nullptr == pScene) return;
	if (E_SceneMode::Play != CSceneManager::GetInstance()->GetSceneMode())
		return;
	
	
	// Canvas내부에 존재하는 최상위 UI 오브젝트들을 가져온다.
	vector<CGameObject*> vecUIObjs;
	GetRootUIObjectsInCanvas(vecUIObjs);
	

	CUI* pPrevFocusedUI = m_pCurFocusedUI;// 이전에 포커싱된 부모 UI

	// 현재 포커싱된 최상위 부모 UI를 가져옴
	E_KeyState eCurKeyState = CKeyManager::GetInstance()->GetKeyState(E_Key::LBUTTON); // Mouse Left Button 상태를 가져옴

	// UI 오브젝트들의 개수를 가져온다.
	int iUICnt = (int)vecUIObjs.size();

	// UI의 개수가 0이면 포커스 해제, 포인트 다운 해제
	if (0 == iUICnt) {
		m_pCurFocusedUI = nullptr;
		m_pPointDownUI = nullptr;
	}

	// TODO (Jang) : 난 이것을 z값을 이용해서 만들 것임.
	// 맨 끝에 있는 UI를 눌렀으면 Press 이벤트가 발생하게 한다.
	for (int i = iUICnt - 1; i >= 0; --i) {
		// Pointer가 On인 상태에서 Press키를 눌렀을 경우
		CUI* pUI = vecUIObjs[i]->GetComponent<CUI>();
		if (pUI) {
			if (pUI->IsActiveClickEvent()) {
				if ((CCollisionManager::GetInstance()->IsCollision(pUI->RectTransform(), MousePosition) && eCurKeyState == E_KeyState::PRESS)) {
					// 현재 포커싱된 UI의오브젝트를 담는다.
					m_pCurFocusedUI = pUI;
					break;
				}
			}
		}
	}

	// 포커싱된 UI가 변경되었으면 그룹 벡터에 가장 뒤로 옮김
	if (m_pCurFocusedUI && pPrevFocusedUI != m_pCurFocusedUI) {
		for (auto iter = vecUIObjs.begin(); iter != vecUIObjs.end(); ++iter) {
			if ((*iter) == m_pCurFocusedUI->GetGameObject()) {
				vecUIObjs.erase(iter);
				break;
			}
		}
		vecUIObjs.push_back(m_pCurFocusedUI->GetGameObject());
	}

	// Focus된 최상위 UI가 있다면
	if(m_pCurFocusedUI && m_pCurFocusedUI->GetGameObject()) {
		// 자식 UI중 타겟팅된 UI를 가져온다. 
		CUI* pTargetUI = m_pCurFocusedUI;

		// 부모로부터 자식 오브젝트 Level Order를 이용하여 찾기 
		queue<CUI*> que;
		que.push(m_pCurFocusedUI);
		while (!que.empty()) {
			CUI* pChildUI = que.front();
			que.pop();

			if (pChildUI) {
				vector<CGameObject*> vecChilds = pChildUI->GetGameObject()->GetChildsObject();

				for (UINT i = 0; i < vecChilds.size(); ++i) {
					if (vecChilds[i]->IsActive())
						que.push(vecChilds[i]->GetComponent<CUI>());
				}

				if (CCollisionManager::GetInstance()->IsCollision(pChildUI->RectTransform(), MousePosition)) {

					pChildUI->m_bIsOn = true;
					if (pTargetUI->m_bIsOn)
						pTargetUI->m_bIsOn = false;
					pTargetUI = pChildUI;
				}
				else {
					pChildUI->m_bIsOn = false;
				}
			}
		}	

		// 마우스 포인터가 UI 외부에 있을 경우
		if (false == CCollisionManager::GetInstance()->IsCollision(pTargetUI->RectTransform(), MousePosition)) {
			if (pTargetUI->m_bIsOn)
				pTargetUI->m_bIsOn = false;
		}
		else { // 마우스 포인터가 UI 내부에 있을 경우
			if (false == pTargetUI->m_bIsOn)
				pTargetUI->m_bIsOn = true;

			if (InputKeyPress(E_Key::LBUTTON)) {
				m_pPointDownUI = pTargetUI;
				pTargetUI->OnPointerDown();
				//_tcprintf(_T("PointerDown %s\n"), pTargetUI->GetGameObject()->GetName().c_str());
				pTargetUI->m_bIsDown = true;
			}
		}

		if (InputKeyRelease(E_Key::LBUTTON)) {
			if (nullptr != m_pPointDownUI) {
				m_pPointDownUI->OnPointerUp();
				//_tcprintf(_T("PointerUp %s\n"), m_pPointDownUI->GetGameObject()->GetName().c_str());

				if (pTargetUI->m_bIsDown && InputKeyRelease(E_Key::LBUTTON)) { //pTargetUI == m_pPointDownUI
					pTargetUI->OnPointerClick();
					//_tcprintf(_T("PointerClick %s\n"), pTargetUI->GetGameObject()->GetName().c_str());

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
				//CUI* pParentUI = (CUI*)vecUIObjs[i];
				CUI* pParentUI = vecUIObjs[i]->GetComponent<CUI>();
				assert(pParentUI);
				assert(pParentUI->GetGameObject());
				if (pParentUI->IsActiveClickEvent()) {
					// 영역에 들어온게 있다면
					if (CCollisionManager::GetInstance()->IsCollision(pParentUI->RectTransform(), MousePosition)) {
						m_pCurFocusedUI = pParentUI;
						break;
					}
				}
			}
		}
	}
}

void CUIManager::GetRootUIObjectsInCanvas(vector<CGameObject*>& _vecRootObjs)
{
	_vecRootObjs.clear();
	CScene* pScene = CSceneManager::GetInstance()->GetCurScene();
	// 현재 씬에서 UI레이어를 가진 최상위 오브젝트들을 가져온다
	vector<CGameObject*> vecCanvas;
	pScene->GetRootGameObjects(vecCanvas, NUM_LAYER_UI);

	// Canvas내부에 존재하는 최상위 UI 오브젝트들을 가져온다.
	for (UINT i = 0; i < vecCanvas.size(); ++i) {
		const vector<CGameObject*>& vecChilds = vecCanvas[i]->GetChildsObject();
		for (UINT j = 0; j < vecChilds.size(); ++j) {
			if (!vecChilds[i]->IsActive())
				continue;
			// CUI이면서 CanvasRenderer가 존재하면 vector에 넣어줌
			if (vecChilds[j]->CanvasRenderer() && vecChilds[j]->GetComponent<CUI>())
				_vecRootObjs.push_back(vecChilds[j]);
		}
	}
}

bool CUIManager::IsMousePointInUI()
{
	vector<CGameObject*> vecUIObjs;
	GetRootUIObjectsInCanvas(vecUIObjs);

	queue<CUI*> que;
	for (UINT i = 0; i < vecUIObjs.size(); ++i)
		que.push(vecUIObjs[i]->GetComponent<CUI>());
	
	while (!que.empty()) {
		CUI* pUI = que.front();
		que.pop();
		if (CCollisionManager::GetInstance()->IsCollision(pUI->RectTransform(), MousePosition))
			return true;

		const vector<CGameObject*>& vecChilds = pUI->GetGameObject()->GetChildsObject();
		for (UINT i = 0; i < vecChilds.size(); ++i) {
			if(vecChilds[i]->IsActive())
				que.push(vecChilds[i]->GetComponent<CUI>());
		}
	}
	return false;
}