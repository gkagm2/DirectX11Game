#include "pch.h"
#include "CCursorScript.h"
#include <Engine\CRenderManager.h>

CCursorScript::CCursorScript() :
	CScript((UINT)SCRIPT_TYPE::CURSORSCRIPT),
	m_iCurIdx(0)
{
	assert(nullptr && _T("상속받아 사용하기"));
}

CCursorScript::CCursorScript(SCRIPT_TYPE _eType) :
	CScript((UINT)_eType),
	m_iCurIdx(0)
{
}

CCursorScript::~CCursorScript()
{
}

void CCursorScript::Awake()
{
	if (!MeshRenderer()) {
		GetGameObject()->AddComponent<CMeshRenderer>();
	}
	m_pCursorMtrl = MeshRenderer()->GetClone_NoSave();
}

void CCursorScript::LateUpdate()
{
	CCamera* pCam = CRenderManager::GetInstance()->GetMainCamera();
	if (!pCam)
		return;
	if (E_ProjectionType::Orthographic == pCam->GetProjectionType()) {
		const Vector2& vMousePos = MousePosition;
		Vector3 vWorldPos = pCam->GetScreenToWorld2DPosition(vMousePos);
		vWorldPos.z += 10.f;
		Transform()->SetLocalPosition(vWorldPos);
	}
}

void CCursorScript::SetCurCursor(int _iCursorIdx)
{
	m_iCurIdx = _iCursorIdx;
	if (m_vecCursorInfo.size() > 0 && m_iCurIdx < m_vecCursorInfo.size()) {
		const TCursorInfo& info = m_vecCursorInfo[m_iCurIdx];
		// Material에다가 넣기
		m_pCursorMtrl->SetData(E_ShaderParam::Texture_0, m_vecCursorInfo[m_iCurIdx].pCursorTex.Get());
	}
}