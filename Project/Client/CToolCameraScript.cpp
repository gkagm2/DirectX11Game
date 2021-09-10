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
	
	CCamera* pToolCam = CRenderManager::GetInstance()->GetToolCamera();
	if (!pToolCam)
		return;

	// Orthographic�� ���
	if (E_ProjectionType::Orthographic == pToolCam->GetProjectionType()) {
		UpdateOrthographic(pToolCam);
	}
	// Perspective�� ���
	else if (E_ProjectionType::Perspective == pToolCam->GetProjectionType()) {
		UpdatePerspective(pToolCam);
	}
}

void CToolCameraScript::UpdateOrthographic(CCamera* _pCamera)
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

void CToolCameraScript::UpdatePerspective(CCamera* _pCamera)
{
	Vector3 vPos = Transform()->GetLocalPosition();
	Vector3 vRot = Transform()->GetLocalRotation();
	CCamera* pToolCam = _pCamera;

	// ���콺�� �̿��� ȸ��
	static Vector2 vPrevMousePos;
	static Vector3 vPrevCamRot;
	if (InputKeyPress(E_Key::RBUTTON)) {
		vPrevMousePos = MousePosition;
		vPrevCamRot = Transform()->GetLocalRotation();
	}
	if (InputKeyHold(E_Key::RBUTTON)) {
		// �����¿�
		// curPos - prevPos�� x���̰� ����̸� ���������� �̵��� -> cam y�� -���� ȸ��
		// curPos - prevPos�� y���̰� ����̸� ���� �̵��� -> cam z�� +���� ȸ��
		Vector2 vCurMousePos = MousePosition;
		Vector2 offset = (vPrevMousePos - vCurMousePos) * 0.01f;
		Vector3 vResultRot = { vPrevCamRot.x - offset.y, vPrevCamRot.y - offset.x, vPrevCamRot.z };

		if (InputKeyHold(E_Key::Q)) {
			vPrevCamRot.z += XM_PI * DT;
		}
		else if (InputKeyHold(E_Key::E)) {
			vPrevCamRot.z -= XM_PI * DT;
		}

		Transform()->SetLocalRotation(vResultRot);
	}

	// World Space�� ��ȯ�ؼ� ���� ��
	float fMoveSpeed = 30.f * DT;
	Vector3 vWorldRot = Transform()->GetLocalRotation();


	if (InputKeyHold(E_Key::Q)) {
		vRot.z += XM_PI * DT;
	}
	else if (InputKeyHold(E_Key::E)) {
		vRot.z -= XM_PI * DT;
	}
	else if (InputKeyHold(E_Key::A)) {
		vPos.x -= 30.f * DT;
	}
	else if (InputKeyHold(E_Key::D)) {
		vPos.x += 30.f * DT;
	}
	else if (InputKeyHold(E_Key::W)) {
		vPos.z += 30.f * DT;
	}
	else if (InputKeyHold(E_Key::S)) {
		vPos.z -= 30.f * DT;
	}
	Transform()->SetLocalPosition(vPos);
	//Transform()->SetLocalPosition(vPos);
	//Transform()->SetLocalRotation(vRot);
}
