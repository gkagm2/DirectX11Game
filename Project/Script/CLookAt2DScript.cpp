#include "pch.h"
#include "CLookAt2DScript.h"

CLookAt2DScript::CLookAt2DScript() :
	CScript((UINT)SCRIPT_TYPE::LOOKAT2DSCRIPT),
	m_pTargetObj(nullptr),
	m_eForwardType(E_ForwardType::X)
{
	AddParam(TScriptParam{ _T("Target Obj") ,E_ScriptParam::GAMEOBJ, &m_pTargetObj });
}

CLookAt2DScript::~CLookAt2DScript()
{
	
}

void CLookAt2DScript::Start()
{
}

void CLookAt2DScript::Update()
{
	if (!m_pTargetObj) return;
	if (!m_pTargetObj->Transform()) return;

	CCamera* pCurCam = CRenderManager::GetInstance()->GetMainCamera();
	if (!pCurCam)
		return;
	if (E_ProjectionType::Orthographic == pCurCam->GetProjectionType()) {

		Vector2 vmousepos = MousePosition;
		Vector3 vpos = pCurCam->GetScreenToWorld2DPosition(vmousepos);

		m_pTargetObj->Transform()->LookAt2D(Vector2(vpos));
	}
}

bool CLookAt2DScript::SaveToScene(FILE* _pFile)
{
	FWrite(m_eForwardType, _pFile);
	FWriteObj(m_pTargetObj, _pFile);
	return true;
}

bool CLookAt2DScript::LoadFromScene(FILE* _pFile)
{
	FRead(m_eForwardType, _pFile);
	FReadObj(&m_pTargetObj, _pFile);
	return true;
}
