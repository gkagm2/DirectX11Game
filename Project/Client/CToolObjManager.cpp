#include "pch.h"
#include "CToolObjManager.h"
#include "CGameObjectEx.h"
#include "CCameraEx.h"
#include "CToolCameraScript.h"
#include "COrthogonalLineScript.h"
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
	{
		CGameObjectEx* pToolCamObj = new CGameObjectEx;
		pToolCamObj->SetName(STR_TOOL_OBJ_NAME_ToolCamera);
		pToolCamObj->AddComponent<CTransform>();
		pToolCamObj->AddComponent<CCameraEx>();
		pToolCamObj->AddComponent<CToolCameraScript>();

		pToolCamObj->Camera()->SetProjectionType(E_ProjectionType::Perspective);
		pToolCamObj->Camera()->SetLayerCheckAll();
		pToolCamObj->Camera()->SetLayerCheck(NUM_LAYER_UI, false);
		pToolCamObj->Camera()->SetSize(0.02f);
		_AddGameObjectEx(pToolCamObj);
	}
	
	{
		CGameObjectEx* pToolUICamObj = new CGameObjectEx;
		pToolUICamObj->SetName(STR_TOOL_OBJ_NAME_ToolUICamera);
		pToolUICamObj->AddComponent<CTransform>();
		pToolUICamObj->AddComponent<CCameraEx>();

		pToolUICamObj->Camera()->SetProjectionType(E_ProjectionType::Orthographic);
		pToolUICamObj->Camera()->SetLayerCheckAllUnActive();
		pToolUICamObj->Camera()->SetLayerCheck(NUM_LAYER_UI, true);
		pToolUICamObj->Camera()->SetSize(1.f);
		pToolUICamObj->Transform()->SetLocalPosition(Vector3(9999.f, 9999.f, 0.f));
		_AddGameObjectEx(pToolUICamObj);
	}

	// Orthogonal AxisCoordinate
	{
		CGameObjectEx* pOrthogonalAxisCoordinate = new CGameObjectEx;
		pOrthogonalAxisCoordinate->SetName(STR_TOOL_OBJ_NAME_ToolOrthogonalAxisCoordinate);
		pOrthogonalAxisCoordinate->AddComponent<CTransform>();
		pOrthogonalAxisCoordinate->AddComponent<COrthogonalLineScript>();
		pOrthogonalAxisCoordinate->AddComponent<CMeshRenderer>();
		_AddGameObjectEx(pOrthogonalAxisCoordinate);
	}

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