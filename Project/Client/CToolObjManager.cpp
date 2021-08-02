#include "pch.h"
#include "CToolObjManager.h"
#include "CGameObjectEx.h"
#include "CCameraEx.h"
#include "CToolCameraScript.h"
#include <Engine\CRenderManager.h>

CToolObjManager::CToolObjManager()
{

}
CToolObjManager::~CToolObjManager()
{
	Safe_Delete_Vector(m_vecToolObjs);
}

void CToolObjManager::Init()
{
	CGameObjectEx* pToolCamObj = new CGameObjectEx;
	pToolCamObj->AddComponent<CTransform>();
	pToolCamObj->AddComponent<CCameraEx>();
	pToolCamObj->AddComponent<CToolCameraScript>();

	pToolCamObj->Camera()->SetProjectionType(E_ProjectionType::Orthographic);
	pToolCamObj->Camera()->SetLayerCheckAll();
	_AddGameObjectEx(pToolCamObj);

	for (UINT i = 0; i < m_vecToolObjs.size(); ++i)
		m_vecToolObjs[i]->Awake();
	for (UINT i = 0; i < m_vecToolObjs.size(); ++i)
		m_vecToolObjs[i]->Start();
}

void CToolObjManager::Progress()
{
	for (UINT i = 0; i < m_vecToolObjs.size(); ++i)
		m_vecToolObjs[i]->PrevUpdate();
	for (UINT i = 0; i < m_vecToolObjs.size(); ++i)
		m_vecToolObjs[i]->Update();
	for (UINT i = 0; i < m_vecToolObjs.size(); ++i)
		m_vecToolObjs[i]->LateUpdate();

	CRenderManager::GetInstance()->UnRegisterToolCamera();

	for (UINT i = 0; i < m_vecToolObjs.size(); ++i)
		m_vecToolObjs[i]->FinalUpdate();
}