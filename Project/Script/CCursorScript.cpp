#include "pch.h"
#include "CCursorScript.h"
#include <Engine\CObjectManager.h>
#include <Engine\CImageUI.h>
#include <Engine\CUI.h>

CCursorScript::CCursorScript() :
	CScript((UINT)SCRIPT_TYPE::CURSORSCRIPT),
	m_ePivot{E_Pivot::Middle}
{
}
CCursorScript::~CCursorScript()
{
}

void CCursorScript::Awake()
{
	ImageUI()->ActiveClickEvent(false);
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
		Vector2 vHotSpotOffset = {};
		switch (m_ePivot) {
		case E_Pivot::LeftTop:
			vHotSpotOffset.x = RectTransform()->GetWidth() * 0.5f;
			vHotSpotOffset.y = -RectTransform()->GetHeight() * 0.5f;
			break;
		case E_Pivot::Middle:
			break;
		default:
			assert(nullptr);
			break;
		}

		vHotSpotOffset += m_vHotSpotOffset;
		RectTransform()->SetLocalPosition(vWorldPos - vOffsetPos + vHotSpotOffset);
	}
}

void CCursorScript::SetCurCursor(SharedPtr<CTexture> _pTex, const Vector2& _vHotSpotOffset)
{
	m_pCursorMtrl->SetData(E_ShaderParam::Texture_0, _pTex.Get());
	m_vHotSpotOffset = _vHotSpotOffset;
}