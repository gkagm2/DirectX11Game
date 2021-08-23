#include "pch.h"
#include "CCameraFollowerScript.h"
#include <Engine\CRenderManager.h>

CCameraFollowerScript::CCameraFollowerScript() :
	CScript((UINT)SCRIPT_TYPE::CAMERAFOLLOWERSCRIPT),
	m_pTargetObj(nullptr),
	m_fMouseScrollSpeed(18.f)
{
}

CCameraFollowerScript::~CCameraFollowerScript()
{
}

void CCameraFollowerScript::Awake()
{
	assert(Camera());
	m_pTargetObj = FIND_GameObject(_T("Player"));
	//assert(m_pTargetObj);
	if (nullptr == m_pTargetObj)
		_tcprintf(_T("[Warning]CCameraFollowerScript Å¸°Ù ÄÄÆ÷³ÍÆ®°¡ ¾øÀ½\n"));
}

void CCameraFollowerScript::Update()
{
	if (nullptr == m_pTargetObj || 
		nullptr == Camera())
		return;

	Vector3 vTargetPosition = m_pTargetObj->Transform()->GetPosition();
	vTargetPosition.z = m_pTargetObj->Transform()->GetPosition().z;
	Transform()->SetLocalPosition(vTargetPosition);

	CMouseEvent evt = CMouseManager::GetInstance()->GetMouseEvent();
	if (E_MouseEventType::WheelUp == evt.GetType() ||
		E_MouseEventType::WheelDown == evt.GetType()) {
		if(E_ProjectionType::Orthographic == Camera()->GetProjectionType())
		Zoom();
	}
}

void CCameraFollowerScript::Zoom()
{
	float fY = MouseScrollDelta * m_fMouseScrollSpeed;
	float fSize = Camera()->GetSize();
	if (fY == 0.f)
		fY = 0.001f;
	fSize += fY;
	fSize = max(0.001f, fSize);
	Camera()->SetSize(fSize);
}