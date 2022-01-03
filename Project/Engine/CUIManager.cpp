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
	// ���� ���� �����´�
	CScene* pScene = CSceneManager::GetInstance()->GetCurScene();
	if (nullptr == pScene) return;
	if (E_SceneMode::Play != CSceneManager::GetInstance()->GetSceneMode())
		return;
	
	
	// Canvas���ο� �����ϴ� �ֻ��� UI ������Ʈ���� �����´�.
	vector<CGameObject*> vecUIObjs;
	GetRootUIObjectsInCanvas(vecUIObjs);
	

	CUI* pPrevFocusedUI = m_pCurFocusedUI;// ������ ��Ŀ�̵� �θ� UI

	// ���� ��Ŀ�̵� �ֻ��� �θ� UI�� ������
	E_KeyState eCurKeyState = CKeyManager::GetInstance()->GetKeyState(E_Key::LBUTTON); // Mouse Left Button ���¸� ������

	// UI ������Ʈ���� ������ �����´�.
	int iUICnt = (int)vecUIObjs.size();

	// UI�� ������ 0�̸� ��Ŀ�� ����, ����Ʈ �ٿ� ����
	if (0 == iUICnt) {
		m_pCurFocusedUI = nullptr;
		m_pPointDownUI = nullptr;
	}

	// TODO (Jang) : �� �̰��� z���� �̿��ؼ� ���� ����.
	// �� ���� �ִ� UI�� �������� Press �̺�Ʈ�� �߻��ϰ� �Ѵ�.
	for (int i = iUICnt - 1; i >= 0; --i) {
		// Pointer�� On�� ���¿��� PressŰ�� ������ ���
		CUI* pUI = vecUIObjs[i]->GetComponent<CUI>();
		if (pUI) {
			if (pUI->IsActiveClickEvent()) {
				if ((CCollisionManager::GetInstance()->IsCollision(pUI->RectTransform(), MousePosition) && eCurKeyState == E_KeyState::PRESS)) {
					// ���� ��Ŀ�̵� UI�ǿ�����Ʈ�� ��´�.
					m_pCurFocusedUI = pUI;
					break;
				}
			}
		}
	}

	// ��Ŀ�̵� UI�� ����Ǿ����� �׷� ���Ϳ� ���� �ڷ� �ű�
	if (m_pCurFocusedUI && pPrevFocusedUI != m_pCurFocusedUI) {
		for (auto iter = vecUIObjs.begin(); iter != vecUIObjs.end(); ++iter) {
			if ((*iter) == m_pCurFocusedUI->GetGameObject()) {
				vecUIObjs.erase(iter);
				break;
			}
		}
		vecUIObjs.push_back(m_pCurFocusedUI->GetGameObject());
	}

	// Focus�� �ֻ��� UI�� �ִٸ�
	if(m_pCurFocusedUI && m_pCurFocusedUI->GetGameObject()) {
		// �ڽ� UI�� Ÿ���õ� UI�� �����´�. 
		CUI* pTargetUI = m_pCurFocusedUI;

		// �θ�κ��� �ڽ� ������Ʈ Level Order�� �̿��Ͽ� ã�� 
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

		// ���콺 �����Ͱ� UI �ܺο� ���� ���
		if (false == CCollisionManager::GetInstance()->IsCollision(pTargetUI->RectTransform(), MousePosition)) {
			if (pTargetUI->m_bIsOn)
				pTargetUI->m_bIsOn = false;
		}
		else { // ���콺 �����Ͱ� UI ���ο� ���� ���
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
	else { // ��Ŀ�̵Ȱ� �ƹ��͵� ������
		if (InputKeyPress(E_Key::LBUTTON)) { // ���� ���콺 ��ư�� �����ٸ�
			// �������� ���� UI�� �ִ��� üũ�Ѵ�.
			for (UINT i = 0; i < vecUIObjs.size(); ++i) {
				//CUI* pParentUI = (CUI*)vecUIObjs[i];
				CUI* pParentUI = vecUIObjs[i]->GetComponent<CUI>();
				assert(pParentUI);
				assert(pParentUI->GetGameObject());
				if (pParentUI->IsActiveClickEvent()) {
					// ������ ���°� �ִٸ�
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
	// ���� ������ UI���̾ ���� �ֻ��� ������Ʈ���� �����´�
	vector<CGameObject*> vecCanvas;
	pScene->GetRootGameObjects(vecCanvas, NUM_LAYER_UI);

	// Canvas���ο� �����ϴ� �ֻ��� UI ������Ʈ���� �����´�.
	for (UINT i = 0; i < vecCanvas.size(); ++i) {
		const vector<CGameObject*>& vecChilds = vecCanvas[i]->GetChildsObject();
		for (UINT j = 0; j < vecChilds.size(); ++j) {
			if (!vecChilds[i]->IsActive())
				continue;
			// CUI�̸鼭 CanvasRenderer�� �����ϸ� vector�� �־���
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