#include "pch.h"
#include "CToolCameraScript.h"
#include <Engine\CSceneManager.h>
#include <Engine\CMouseManager.h>
#include <Engine\CRenderManager.h>
#include <Engine\CKeyManager.h>
#include <Engine\CMouseManager.h>

#include "CImGuiManager.h"
#include "DebugGUI.h"

#include "CCameraEx.h"

CToolCameraScript::CToolCameraScript() :
	CScript(-1),
	m_fSpeed(0.5f),
	m_fMouseScrollSpeed(0.8f),
	m_fDragSpeed(0.02f),
	m_fPrefSize{},
	m_vPrevToolCamPos{}
{
}

CToolCameraScript::~CToolCameraScript()
{
}

void CToolCameraScript::Start()
{
	GetGameObject()->Transform()->SetLocalPosition(Vector3{});
}

void CToolCameraScript::Update()
{
	if (CSceneManager::GetInstance()->GetSceneMode() == E_SceneMode::Play)
		return;
	
	{
		Vector3 vPos = Transform()->GetLocalPosition();

		if (InputKeyPress(E_Key::MBUTTON) && InputKeyHold(E_Key::LCtrl)) {
			CCamera* pToolCam = CRenderManager::GetInstance()->GetToolCamera();
			m_vPrevPos = MousePosition;// pToolCam->GetScreenToWorld2DPosition(MousePosition);

			if (E_ProjectionType::Orthographic == pToolCam->GetProjectionType()) {
				m_fPrefSize = pToolCam->GetSize();
			}
		}
		else if (InputKeyHold(E_Key::MBUTTON) && InputKeyHold(E_Key::LCtrl)) {
			CCamera* pToolCam = CRenderManager::GetInstance()->GetToolCamera();
			Vector2 vCurPos = MousePosition;//pToolCam->GetScreenToWorld2DPosition(MousePosition);
			Vector2 vMoveOffset{};
			float fDistance = m_vPrevPos.y - vCurPos.y;
			float fOffSet = fDistance * m_fDragSpeed;
			if (E_ProjectionType::Orthographic == pToolCam->GetProjectionType()) {
				float fSize = m_fPrefSize + fOffSet;
				if (0.f == fSize)
					fSize = 0.001f;
				pToolCam->SetSize(fSize);
			}
		}
		else if (InputKeyPress(E_Key::MBUTTON))
		{
			m_vPrevPos = MousePosition;
		}
		else if (InputKeyHold(E_Key::MBUTTON)) {
			m_vMoveOffset = MousePosition - m_vPrevPos;
			m_vMoveOffset.x *= -1;
			vPos = vPos - m_vMoveOffset * DT * m_fSpeed;
		}
		else if (InputKeyRelease(E_Key::MBUTTON)) {
		}
		CMouseEvent evt = CMouseManager::GetInstance()->GetMouseEvent();
		if (E_MouseEventType::WheelUp == evt.GetType() ||
			E_MouseEventType::WheelDown == evt.GetType())
		{
			CCamera* pToolCam = CRenderManager::GetInstance()->GetToolCamera();
			if (E_ProjectionType::Orthographic == pToolCam->GetProjectionType()) {
				float y = MouseScrollDelta * m_fMouseScrollSpeed;
				float fSize = pToolCam->GetSize();
				if (y == 0.f)
					y = 0.001f;
				fSize += y;
				pToolCam->SetSize(fSize);
			}
		}

		Transform()->SetLocalPosition(vPos);
	}

	if (InputKeyHold(E_Key::LCtrl) && InputKeyPress(E_Key::RBUTTON)) {
		CCamera* pToolCam = CRenderManager::GetInstance()->GetToolCamera();
		m_vPrevPos = MousePosition;// pToolCam->GetScreenToWorld2DPosition(MousePosition);

		if (E_ProjectionType::Orthographic == pToolCam->GetProjectionType()) {
			m_vPrevToolCamPos = pToolCam->Transform()->GetLocalPosition();
		}
	}
	if (InputKeyHold(E_Key::LCtrl) && InputKeyHold(E_Key::RBUTTON)) {
		CCamera* pToolCam = CRenderManager::GetInstance()->GetToolCamera();
		Vector2 vCurPos = MousePosition;//pToolCam->GetScreenToWorld2DPosition(MousePosition);
		Vector2 vMoveOffset{};
		vMoveOffset.x = m_vPrevPos.x - vCurPos.x;
		vMoveOffset.y = vCurPos.y - m_vPrevPos.y;

		if (E_ProjectionType::Orthographic == pToolCam->GetProjectionType()) {
			pToolCam->Transform()->SetLocalPosition(m_vPrevToolCamPos + vMoveOffset);
		}
	}
	if (InputKeyPress(E_Key::F2)) {
		CCamera* pToolCam = CRenderManager::GetInstance()->GetToolCamera();
		pToolCam->Transform()->SetLocalPosition(Vector3{});
	}

	CCamera* pToolCam = CRenderManager::GetInstance()->GetToolCamera();
	if (pToolCam) {
		_tcprintf(_T("[%.2f %.2f %.2f]\n"), pToolCam->Transform()->GetPosition().x, pToolCam->Transform()->GetPosition().y, pToolCam->Transform()->GetPosition().z);

	}
}