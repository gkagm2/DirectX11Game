#include "pch.h"
#include "CMouseLookAt_bu.h"

CMouseLookAt_bu::CMouseLookAt_bu() :
	CScript((UINT)SCRIPT_TYPE::MOUSELOOKAT_BU)
{
}

void CMouseLookAt_bu::Update()
{
	CCamera* pCurCam = CRenderManager::GetInstance()->GetMainCamera();
	if (!pCurCam)
		return;
	if (E_ProjectionType::Orthographic == pCurCam->GetProjectionType()) {
		Vector2 vmousepos = MousePosition;
		tstring str = to_tstring(vmousepos.x) + _T(", ") + to_tstring(vmousepos.y);
		CFontManager::GetInstance()->DrawFont(str.c_str(), 30, 100, 12, FONT_RGBA(200, 30, 30, 255), FW1_TEXT_FLAG::FW1_LEFT);
		Vector3 vpos = pCurCam->GetScreenToWorld2DPosition(vmousepos);
		Transform()->LookAt2D(Vector2(vpos));
	}
}