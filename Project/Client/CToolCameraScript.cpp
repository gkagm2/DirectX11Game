#include "pch.h"
#include "CToolCameraScript.h"
#include <Engine\CSceneManager.h>
#include <Engine\CMouseManager.h>
#include "imgui.h"

CToolCameraScript::CToolCameraScript() :
	CScript(-1),
	m_fSpeed(40.f)
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
	if (CMouseEvent::EventType::WheelUp == evt.GetType())
	{
		int a = 3;
	}

	Transform()->SetLocalPosition(vPos);
}