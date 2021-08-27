#include "pch.h"
#include "CLookAtScript.h"
#include <Engine\CRenderManager.h>

CLookAtScript::CLookAtScript() :
	CScript((UINT)SCRIPT_TYPE::LOOKATSCRIPT)
{
}

CLookAtScript::~CLookAtScript()
{
}

void CLookAtScript::Start()
{
}

void CLookAtScript::Update()
{
	CCamera* pCam = CRenderManager::GetInstance()->GetMainCamera();
	Vector3 vMouseTargetPos = pCam->GetScreenToWorld2DPosition(MousePosition);

	Vector3 vTargetDir = vMouseTargetPos - Transform()->GetPosition();
	vTargetDir.Normalize();
	
	Vector3 vUpDir = Transform()->GetUpVector();

	float fDot = vTargetDir.Dot(vUpDir);
	float fDegree = acosf(fDot)* CMyMath::Rad2Deg();

	Vector3 vCross = vTargetDir.Cross(vUpDir);
	Vector3 vResultDir{};
	if (vCross.z > 0) {
		vResultDir = Rotate(vUpDir, 0.1f); // 시계
	}
	else if (vCross.z < 0) {
		vResultDir = Rotate(vUpDir, -0.1f); // 반시계
	}

	_tcprintf(_T("T%.2f %.2f %.2f\n"), vTargetDir.x, vTargetDir.y, vTargetDir.z);
	_tcprintf(_T("M%.2f %.2f %.2f\n"), vResultDir.x, vResultDir.y, vResultDir.z);

	Transform()->SetLocalRotationDegree(Vector3(0.f, 0.f, fDegree));
}