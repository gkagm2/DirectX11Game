#include "pch.h"
#include "CToolCameraScript.h"
#include <Engine\CSceneManager.h>

#include "imgui.h"

CToolCameraScript::CToolCameraScript() :
	CScript(-1),
	m_fSpeed(200.f)
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
	if (InputKeyHold(E_Key::UP)) {
		vPos.y += DT * 150.f;
	}
	if (InputKeyHold(E_Key::DOWN)) {
		vPos.y -= DT * 150.f;
	}
	if (InputKeyHold(E_Key::LEFT)) {
		vPos.x -= DT * 150.f;
	}
	if (InputKeyHold(E_Key::RIGHT)) {
		vPos.x += DT * 150.f;
	}
	Transform()->SetLocalPosition(vPos);
}