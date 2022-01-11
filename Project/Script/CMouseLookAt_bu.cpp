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
		Vector3 vpos = pCurCam->GetScreenToWorld2DPosition(vmousepos);
		Transform()->LookAt2D(Vector2(vpos));
	}
}