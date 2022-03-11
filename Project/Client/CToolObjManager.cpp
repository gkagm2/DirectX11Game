#include "pch.h"
#include "CToolObjManager.h"
#include "CGameObjectEx.h"
#include "CCameraEx.h"
#include "CToolCameraScript.h"
#include "CToolDragAndDropScript.h"
#include "COrthogonalLineScript.h"
#include <Engine\CRenderManager.h>

CToolObjManager::CToolObjManager()
{

}
CToolObjManager::~CToolObjManager()
{
	Safe_Delete_Vector(m_vecToolObjs);
	Safe_Delete_Map(m_mapShader);
	Safe_Delete_Map(m_mapMtrl);
}

void CToolObjManager::_CreateToolMaterial()
{
	// TODO (Jang) : MRT 이후 만들기
}

void CToolObjManager::_CreateToolObject()
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
		TClippingPlanes tCP = {};
		tCP.fFar = 99999.f;
		tCP.fNear = 1.f;
		pToolCamObj->Camera()->SetClippingPlanes(tCP);
		pToolCamObj->Camera()->SetRenderFrustum(false);
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
		pToolUICamObj->Transform()->SetLocalPosition(DEFAULT_UI_CAM_POS);
		pToolUICamObj->Camera()->SetRenderFrustum(false);
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
	{
		CGameObjectEx* pDragAndDropObj = new CGameObjectEx;
		pDragAndDropObj->SetName(STR_TOOL_OBJ_NAAME_DragAndDrop);
		pDragAndDropObj->AddComponent<CToolDragAndDropScript>();
		_AddGameObjectEx(pDragAndDropObj);
	}
}

void CToolObjManager::Init()
{
	_CreateToolObject();
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