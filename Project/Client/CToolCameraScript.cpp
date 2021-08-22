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
	m_fSpeed(40.f),
	m_fMouseScrollSpeed(14.f)
{
}

CToolCameraScript::~CToolCameraScript()
{
}

void CToolCameraScript::Update()
{
	if (CSceneManager::GetInstance()->GetSceneMode() == E_SceneMode::Play)
		return;

	Vector3 vPos = Transform()->GetLocalPosition();

	if (InputKeyPress(E_Key::MBUTTON))
	{
		m_vPrevPos = MousePosition;
	}
	if (InputKeyHold(E_Key::MBUTTON)) {
		m_vMoveOffset = MousePosition - m_vPrevPos;
		m_vMoveOffset.x *= -1;
		vPos = vPos - m_vMoveOffset * DT * m_fSpeed;
	}
	if (InputKeyRelease(E_Key::MBUTTON)) {
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