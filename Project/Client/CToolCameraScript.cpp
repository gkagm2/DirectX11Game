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
	m_vPrevToolCamPos{},
	m_bMovementActive{true},
	m_vRotOrtho{},
	m_vRotPerspec{}
{
}

CToolCameraScript::~CToolCameraScript()
{
}

void CToolCameraScript::Start()
{
	GetGameObject()->Transform()->SetLocalPosition(Vector3{0, 0, -500.f});
}

void CToolCameraScript::Update()
{
	if (CSceneManager::GetInstance()->GetSceneMode() == E_SceneMode::Play)
		return;
	
	CCamera* pToolCam = CRenderManager::GetInstance()->GetToolCamera();
	if (!pToolCam)
		return;
	
	if (!IsMovementActive())
		return;

	// Orthographic일 경우
	if (E_ProjectionType::Orthographic == pToolCam->GetProjectionType()) {
		_UpdateOrthographic(pToolCam);
	}
	// Perspective일 경우
	else if (E_ProjectionType::Perspective == pToolCam->GetProjectionType()) {
		_UpdatePerspective(pToolCam);
	}
}

void CToolCameraScript::_UpdateOrthographic(CCamera* _pCamera)
{
	Vector3 vPos = Transform()->GetLocalPosition();
	CCamera* pToolCam = _pCamera;
	{
		if (InputKeyPress(E_Key::MBUTTON) && InputKeyHold(E_Key::LCtrl)) {

			m_vPrevPos = MousePosition;// pToolCam->GetScreenToWorld2DPosition(MousePosition);
			m_fPrefSize = pToolCam->GetSize();
		}
		else if (InputKeyHold(E_Key::MBUTTON) && InputKeyHold(E_Key::LCtrl)) {
			Vector2 vCurPos = MousePosition;//pToolCam->GetScreenToWorld2DPosition(MousePosition);
			Vector2 vMoveOffset{};
			float fDistance = m_vPrevPos.y - vCurPos.y;
			float fOffSet = fDistance * m_fDragSpeed;
			float fSize = m_fPrefSize + fOffSet;
			pToolCam->SetSize(fSize);
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
			float y = MouseScrollDelta * m_fMouseScrollSpeed;
			float fSize = pToolCam->GetSize();
			fSize += y;
			pToolCam->SetSize(fSize);
		}

		Transform()->SetLocalPosition(vPos);
	}

	{
		if (InputKeyHold(E_Key::LCtrl) && InputKeyPress(E_Key::RBUTTON)) {
			m_vPrevPos = MousePosition;// pToolCam->GetScreenToWorld2DPosition(MousePosition);

			if (E_ProjectionType::Orthographic == pToolCam->GetProjectionType()) {
				m_vPrevToolCamPos = pToolCam->Transform()->GetLocalPosition();
			}
		}
		if (InputKeyHold(E_Key::LCtrl) && InputKeyHold(E_Key::RBUTTON)) {
			Vector2 vCurPos = MousePosition;//pToolCam->GetScreenToWorld2DPosition(MousePosition);
			Vector2 vMoveOffset{};
			vMoveOffset.x = m_vPrevPos.x - vCurPos.x;
			vMoveOffset.y = vCurPos.y - m_vPrevPos.y;

			pToolCam->Transform()->SetLocalPosition(m_vPrevToolCamPos + vMoveOffset * pToolCam->GetSize());
		}
		if (InputKeyPress(E_Key::F2)) {
			pToolCam->Transform()->SetLocalPosition(Vector3{});
		}
	}
}

void CToolCameraScript::_UpdatePerspective(CCamera* _pCamera)
{
	Vector3 vPos = Transform()->GetLocalPosition();
	Vector3 vRot = Transform()->GetLocalRotation();
	CCamera* pToolCam = _pCamera;

	
	// 마우스를 이용한 회전
	static Vector2 vPrevMousePos;
	static Vector3 vPrevCamRot;
	if (InputKeyPress(E_Key::RBUTTON)) {
		vPrevMousePos = MousePosition;
		vPrevCamRot = Transform()->GetLocalRotation();
	}
	if (InputKeyHold(E_Key::RBUTTON)) {
		// 상하좌우
		// curPos - prevPos의 x차이가 양수이면 오른쪽으로 이동함 -> cam y축 -방향 회전
		// curPos - prevPos의 y차이가 양수이면 위로 이동함 -> cam z축 +방향 회전
		Vector2 vCurMousePos = MousePosition;
		Vector2 offset = (vPrevMousePos - vCurMousePos) * 0.005f;
		Vector3 vResultRot = { vPrevCamRot.x - offset.y, vPrevCamRot.y - offset.x, vPrevCamRot.z };

		//if (InputKeyHold(E_Key::Q)) {
		//	vPrevCamRot.z += XM_PI * DT;
		//}
		//else if (InputKeyHold(E_Key::E)) {
		//	vPrevCamRot.z -= XM_PI * DT;
		//}

		Transform()->SetLocalRotation(vResultRot);
	}
	

	// 움직임
	float fMoveSpeed = 30.f * DT;
	if (InputKeyHold(E_Key::LShift)) {
		fMoveSpeed *= 30.f;
	}
	if (InputKeyHold(E_Key::E))
		vPos += Transform()->GetLocalUpVector() * fMoveSpeed;
	if (InputKeyHold(E_Key::Q))
		vPos += -Transform()->GetLocalUpVector() * fMoveSpeed;
	if (InputKeyHold(E_Key::A))
		vPos += -Transform()->GetLocalRightVector() * fMoveSpeed;
	if (InputKeyHold(E_Key::D))
		vPos += Transform()->GetLocalRightVector() * fMoveSpeed;
	if (InputKeyHold(E_Key::W))
		vPos += Transform()->GetLocalFrontVector() * fMoveSpeed;
	if (InputKeyHold(E_Key::S))
		vPos += -Transform()->GetLocalFrontVector() * fMoveSpeed;
	Transform()->SetLocalPosition(vPos);
	//Transform()->SetLocalPosition(vPos);
	//Transform()->SetLocalRotation(vRot);
}

void CToolCameraScript::ChangeProjectionType(E_ProjectionType _eType)
{
	E_ProjectionType eCurType = Camera()->GetProjectionType();
	if (E_ProjectionType::Orthographic == eCurType) {
		m_vRotOrtho = Transform()->GetLocalRotation();
	}
	else if (E_ProjectionType::Perspective == eCurType) {
		m_vRotPerspec = Transform()->GetLocalRotation();
	}
	else
		assert(nullptr);

	if (E_ProjectionType::Orthographic == _eType) {
		Camera()->SetProjectionType(_eType);
		Transform()->SetLocalRotation(m_vRotOrtho);
	}

	else if (E_ProjectionType::Perspective == _eType) {
		Camera()->SetProjectionType(_eType);
		Transform()->SetLocalRotation(m_vRotPerspec);
	}
	else
		assert(nullptr);
}
