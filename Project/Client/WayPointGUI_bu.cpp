#include "pch.h"
#include "WayPointGUI_bu.h"
#include <Engine\CWayPoint2D.h>
#include <Engine\CKeyManager.h>
#include <Engine\CRenderManager.h>
#include <Engine\CCamera.h>
#include <Engine\CObjectManager.h>
#include <Engine\CResourceManager.h>
#include <Engine\CRectTransform.h>
#include <Engine\CGameObject.h>
#include <Script\CWayPoint_bu.h>
#include <Engine\CCollider2D.h>

WayPointGUI_bu::WayPointGUI_bu() : 
	m_pWayPointObj{ nullptr },
	m_pWayPoint{ nullptr },
	m_pDragObj{nullptr}
{
}

WayPointGUI_bu::~WayPointGUI_bu()
{
	_Clear();
}

void WayPointGUI_bu::Init()
{
	SetName(STR_GUI_WayPoint_bu);
	SetActive(false);
}

void WayPointGUI_bu::Update()
{
	if (!m_bGUIOpen) {
		_Clear();
		return;
	}

	if (E_SceneMode::Stop != CSceneManager::GetInstance()->GetSceneMode())
		return;
	if(ImGui::Begin(STR_GUI_WayPoint_bu, &m_bGUIOpen)){
		// 켜져있는 상태로 waypoint object를 삭제하면 에러남.
		if (!m_pWayPointObj || !m_pWayPoint) {
			m_pWayPointObj = FIND_GameObject(BUTCHER_ObjName_WayPoint_bu);
			if (m_pWayPointObj)
				m_pWayPoint = m_pWayPointObj->GetComponent<CWayPoint_bu>();
			else
				m_pWayPointObj = _CreateWayPointObj();

			if (!m_pWayPoint) {
				m_pWayPoint = m_pWayPointObj->GetComponent<CWayPoint_bu>();
			}
		}

		CCamera* pMainCam = CRenderManager::GetInstance()->GetMainCamera();
		if (E_ProjectionType::Perspective == pMainCam->GetProjectionType()) {
			pMainCam->SetProjectionType(E_ProjectionType::Orthographic);
		}

		// 어떤 메뉴를 해야되지?
		ImGui::Text("Ctrl : Create Waypoint");
		ImGui::Text("Alt : Delete Waypoint");

		Vector2 vMousePos = MousePosition;
		Vector3 vWorldPos = pMainCam->GetScreenToWorld2DPosition(vMousePos);
		vWorldPos.z = -10.f;

		if (InputKeyPress(E_Key::LBUTTON)) {
			if (m_pDragObj)
				m_pDragObj = nullptr;
			m_pDragObj = _GetClickedObj(vWorldPos);
		}
			
		if (InputKeyHold(E_Key::LBUTTON)) {
			if (m_pDragObj)
				m_pDragObj->Transform()->SetLocalPosition(vWorldPos);
		}
		
		// Create
		if (InputKeyHold(E_Key::LCtrl) && InputKeyPress(E_Key::LBUTTON)) {
			Vector2 vWorldPos2D = vWorldPos.XY();
			CGameObject* pObj = m_pWayPoint->AddWayPoint(vWorldPos2D);
			m_pDragObj = pObj;
		}
		// Delete
		else if (InputKeyHold(E_Key::LAlt) && InputKeyPress(E_Key::LBUTTON)) {
			// 클릭했는지 영역을 알아내야 함.
			CGameObject* pDelObj = _GetClickedObj(vWorldPos);
			if (pDelObj) {
				m_pWayPoint->DeleteWayPoint(pDelObj);
				m_pDragObj = nullptr;
			}
		}

		ImGui::End();
	}
}

void WayPointGUI_bu::_DeletWayPoint(CGameObject* _pWayPointObj)
{
}

CGameObject* WayPointGUI_bu::_CreateWayPointObj()
{
	UINT iLayer = (UINT)E_Layer::WayPoint_Tool;
	CGameObject* pWayPointObj =  CObjectManager::GetInstance()->CreateEmptyGameObject(iLayer);
	pWayPointObj->AddComponent<CWayPoint_bu>();
	pWayPointObj->SetName(BUTCHER_ObjName_WayPoint_bu);
	pWayPointObj->Transform()->SetLocalPosition(Vector3(0.f, 0.f, -10.f));
	return pWayPointObj;
}

CGameObject* WayPointGUI_bu::_GetClickedObj(const Vector3& _vWorldPos)
{
	bool isCollision = false;
	int idx = 0;
	vector<CGameObject*>& vecWayPoints = m_pWayPoint->GetWayPointObjs();
	for (int i = 0; i < vecWayPoints.size(); ++i) {
		CGameObject* pObj = vecWayPoints[i];
		CCollider2D* pCol = pObj->Collider2D();
		isCollision = CCollisionManager::GetInstance()->IsCollision(pCol, _vWorldPos);

		if (isCollision) {
			idx = i;
			break;
		}
	}
	if (isCollision)
		return vecWayPoints[idx];
	return nullptr;
}

void WayPointGUI_bu::_Clear()
{
	m_pWayPointObj = nullptr;
	m_pWayPoint = nullptr;
	m_pDragObj = nullptr;
}
