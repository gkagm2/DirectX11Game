#include "pch.h"
#include "CToolDragAndDropScript.h"
#include <Engine\CComponent.h>
#include <Engine\CSceneManager.h>
#include <Engine\CRenderManager.h>
#include <Engine\CCamera.h>
#include <Engine\CGameObject.h>
#include <Engine\CKeyManager.h>

#include "CImGuiManager.h"
#include "InspectorGUI.h"

CToolDragAndDropScript::CToolDragAndDropScript() :
	CScript(-1),
	m_bClick{ false },
	m_pInspGUI{ nullptr },
	m_pTargetObj{ nullptr },
	m_vPrevPos{},
	m_vCurPos{},
	m_vPrevTargetPos{},
	m_vCurTargetPos{},
	m_fMaxDoublePressCoolTime{0.5f}
{
}

CToolDragAndDropScript::~CToolDragAndDropScript()
{
}


void CToolDragAndDropScript::Start()
{
	CCamera* pToolCam = CRenderManager::GetInstance()->GetToolCamera();

}

void CToolDragAndDropScript::Update()
{
	if (E_SceneMode::Play == CSceneManager::GetInstance()->GetSceneMode())
		return;
	if (!m_pInspGUI) {
		m_pInspGUI = dynamic_cast<InspectorGUI*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_Inspector));
		if (!m_pInspGUI) {
			assert(m_pInspGUI);
			return;
		}
	}
	m_pTargetObj = m_pInspGUI->GetTargetObject();
	
	if (!m_pTargetObj)
		return;
	
	CCamera* pToolCam = CRenderManager::GetInstance()->GetToolCamera();
	if (E_ProjectionType::Orthographic != pToolCam->GetProjectionType())
		return;

	bool bObjDrag =false;
	bool bObjMoveToMousePos = false;
	bool bDoublePress = false;
	bool bCamFocusObj = false;

	if (InputKeyPress(E_Key::LBUTTON) && InputKeyHold(E_Key::M)) {
		m_vPrevPos = pToolCam->GetScreenToWorld2DPosition(MousePosition);
		m_vCurPos = m_vPrevPos;
		m_vPrevTargetPos = m_pTargetObj->Transform()->GetLocalPosition();
		bObjDrag = true;
	}

	if (InputKeyHold(E_Key::LBUTTON) && InputKeyHold(E_Key::M)) {
		m_vCurPos = pToolCam->GetScreenToWorld2DPosition(MousePosition);
		m_vCurTargetPos = m_pTargetObj->Transform()->GetLocalPosition();
		bObjDrag = true;
	}

	if (InputKeyHold(E_Key::LBUTTON) && InputKeyHold(E_Key::F)) {
		bObjMoveToMousePos = true;
	}

	m_fDoublePressCoolTime += DT;
	if (InputKeyPress(E_Key::F)) {
		++m_iPressCnt;
		if (m_fDoublePressCoolTime >= m_fMaxDoublePressCoolTime) {
			m_fDoublePressCoolTime = 0.f;
			m_iPressCnt = 1;
		}
		if (m_fDoublePressCoolTime < m_fMaxDoublePressCoolTime) {
			if (m_iPressCnt == 1) {
			}
			if (m_iPressCnt >= 2) {
				bCamFocusObj = true;
				m_iPressCnt = 0;
				m_fDoublePressCoolTime = m_fMaxDoublePressCoolTime;
			}
		}
		
	}
	
	if (bObjDrag) {
		Vector2 vOffsetPos{ m_vCurPos.x - m_vPrevPos.x, m_vCurPos.y - m_vPrevPos.y };

		m_vCurTargetPos.x = m_vPrevTargetPos.x + vOffsetPos.x;
		m_vCurTargetPos.y = m_vPrevTargetPos.y + vOffsetPos.y;
		m_pTargetObj->Transform()->SetLocalPosition(m_vCurTargetPos);
	}

	if (bObjMoveToMousePos) {
		Vector3 vPos = pToolCam->GetScreenToWorld2DPosition(MousePosition);
		m_vCurTargetPos = m_pTargetObj->Transform()->GetLocalPosition();
		m_vCurTargetPos.x = vPos.x;
		m_vCurTargetPos.y = vPos.y;
		m_pTargetObj->Transform()->SetLocalPosition(m_vCurTargetPos);
	}


	if (bCamFocusObj) {
		Vector3 vCamPos = pToolCam->Transform()->GetLocalPosition();
		m_vCurTargetPos = m_pTargetObj->Transform()->GetLocalPosition();
		vCamPos.x = m_vCurTargetPos.x;
		vCamPos.y = m_vCurTargetPos.y;
		pToolCam->Transform()->SetLocalPosition(vCamPos);
	}
}