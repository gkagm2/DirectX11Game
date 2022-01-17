#include "pch.h"
#include "CRespawnManager_bu.h"

CRespawnManager_bu::CRespawnManager_bu() :
	CScript((UINT)SCRIPT_TYPE::RESPAWNMANAGER_BU)
{
}

void CRespawnManager_bu::Update()
{
}

void CRespawnManager_bu::Awake()
{
	m_vecStages.resize(8);
}

void CRespawnManager_bu::CreateEnemey(const Vector3& _vPos, int _pInsertStageNum)
{
	if (_pInsertStageNum < 0 || _pInsertStageNum >= m_vecStages.size())
		return;
	CCamera* pCam = CRenderManager::GetInstance()->GetMainCamera();
	Vector3 vWorldPos = pCam->GetScreenToWorld2DPosition(MousePosition);
	UINT iLayer = (UINT)E_Layer::Object;
	CObject::InstantiateEvn(m_pPlayerPref, vWorldPos, iLayer);
}

void CRespawnManager_bu::CreatePlayer(const Vector3& _vPos, int _pInsertStageNum)
{
	if (_pInsertStageNum < 0 || _pInsertStageNum >= m_vecStages.size())
		return;

}