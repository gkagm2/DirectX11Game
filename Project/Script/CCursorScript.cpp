#include "pch.h"
#include "CCursorScript.h"
#include <Engine\CObjectManager.h>

CCursorScript::CCursorScript() :
	CScript((UINT)SCRIPT_TYPE::CURSORSCRIPT)
{
}
CCursorScript::~CCursorScript()
{
}

void CCursorScript::Awake()
{
}

void CCursorScript::LateUpdate()
{
	CCamera* pCam = CRenderManager::GetInstance()->GetUICamera();
	if (!pCam)
		return;
	if (E_ProjectionType::Orthographic == pCam->GetProjectionType()) {
		const Vector2& vMousePos = MousePosition;
		Vector3 vWorldPos = pCam->GetScreenToWorld2DPosition(vMousePos);
		Vector3 vOffsetPos = {};
		vWorldPos.z = pCam->Transform()->GetLocalPosition().z + 10.f;
		CGameObject* pParentObj = GetGameObject()->GetParentObject();
		if (pParentObj) {
			vOffsetPos = pParentObj->RectTransform()->GetPosition();
		}
		RectTransform()->SetLocalPosition(vWorldPos - vOffsetPos);
	}
}

void CCursorScript::SetCurCursor(SharedPtr<CTexture> _pTex, const Vector2 _vSize)
{
	m_pCursorMtrl->SetData(E_ShaderParam::Texture_0, _pTex.Get());
}
