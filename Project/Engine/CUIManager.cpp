#include "pch.h"

#include "CKeyManager.h"

#include "CUIManager.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CUI.h"

CUIManager::CUIManager() :
	m_pCurFocusedUI(nullptr),
	m_pPointDownUI(nullptr)
{
}

CUIManager::~CUIManager() {

}

void CUIManager::Update()
{
	CScene* pScene = CSceneManager::GetInstance()->GetCurScene();
	if (nullptr == pScene) return;
	vector<CGameObject*> vecUIObjs;
	pScene->GetGameObjects(vecUIObjs, NUM_LAYER_UI);

	CUI* pPrevFocusedUI = m_pCurFocusedUI;// ������ ��Ŀ�̵� �θ� UI

	// ���� ��Ŀ�̵� �ֻ��� �θ� UI�� ������
	E_KeyState eCurKeyState = CKeyManager::GetInstance()->GetKeyState(E_Key::LBUTTON); // Mouse Left Button ���¸� ������

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

	// ��Ŀ�̵� UI�� ����Ǿ����� �׷� ���Ϳ� ���� �ڷ� �ű�
	if (pPrevFocusedUI != m_pCurFocusedUI) {
		for (auto iter = vecUIObjs.begin(); iter != vecUIObjs.end(); ++iter) {
			if ((*iter) == m_pCurFocusedUI->GetGameObject()) {
				vecUIObjs.erase(iter);
				break;
			}
		}
		vecUIObjs.push_back(m_pCurFocusedUI->GetGameObject());
	}

	// Focus�� �ֻ��� UI�� �ִٸ�
	if(m_pCurFocusedUI) {
		// �ڽ� UI�� Ÿ���õ� UI�� �����´�. 
		CUI* pTargetUI = m_pCurFocusedUI;

		// �θ�κ��� �ڽ� ������Ʈ Level Order�� �̿��Ͽ� ã�� 
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

		// ���콺 �����Ͱ� UI �ܺο� ���� ���
		if (false == pTargetUI->IsPointerOn(MousePosition)) {
			if (pTargetUI->m_bIsOn)
				pTargetUI->m_bIsOn = false;
		}
		else { // ���콺 �����Ͱ� UI ���ο� ���� ���
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
	else { // ��Ŀ�̵Ȱ� �ƹ��͵� ������
		if (InputKeyPress(E_Key::LBUTTON)) { // ���� ���콺 ��ư�� �����ٸ�
			// �������� ���� UI�� �ִ��� üũ�Ѵ�.
			for (UINT i = 0; i < vecUIObjs.size(); ++i) {
				CUI* pParentUI = (CUI*)vecUIObjs[i];
				// ������ ���°� �ִٸ�
				if (pParentUI->IsPointerOn(MousePosition)) {
					m_pCurFocusedUI = pParentUI;
					break;
				}
			}
		}
	}
}

bool CUIManager::IsMousePointInUI()
{
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

	return false;
}