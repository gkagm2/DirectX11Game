#include "pch.h"
#include "CCameraFollowerScript.h"
#include <Engine\CRenderManager.h>

CCameraFollowerScript::CCameraFollowerScript() :
	CScript((UINT)SCRIPT_TYPE::CAMERAFOLLOWERSCRIPT),
	m_pTargetCamObj(nullptr)
{
}

CCameraFollowerScript::~CCameraFollowerScript()
{
}

void CCameraFollowerScript::Start()
{
	CCamera* pCamera =GetGameObject()->GetComponent<CCamera>();
	if (pCamera) {
		m_pTargetCamObj = GetGameObject();
	}
	else {
		CGameObject* pTargetCamObj = CRenderManager::GetInstance()->GetMainCamera()->GetGameObject();
		if (pTargetCamObj)
			m_pTargetCamObj = pTargetCamObj;
	}
}

void CCameraFollowerScript::Update()
{
	if (nullptr == m_pTargetCamObj)
		return;

	Vector3 vPosition = Transform()->GetPosition();
	vPosition.z = m_pTargetCamObj->Transform()->GetPosition().z;
	m_pTargetCamObj->Transform()->SetLocalPosition(vPosition);
	
}