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
	m_pPressObj{ nullptr },
	m_pReleaseObj{ nullptr },
	m_pClickedObj{nullptr}
{
}

WayPointGUI_bu::~WayPointGUI_bu()
{
	if (m_pWayPoint) {
		delete m_pWayPoint;
		m_pWayPoint = nullptr;
	}
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
		if (!_CheckAndGenerateWayPoint()) {
			ImGui::End();
			return;
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

		// Create
		if (InputKeyHold(E_Key::LCtrl) && InputKeyPress(E_Key::LBUTTON)) {
			Vector2 vWorldPos2D = vWorldPos.XY();
			m_pWayPoint->AddWayPoint(vWorldPos2D);
		}
		// Delete
		else if (InputKeyHold(E_Key::LAlt) && InputKeyPress(E_Key::LBUTTON)) {
			// 클릭했는지 영역을 알아내야 함.
			if (m_pClickedObj)
				m_pWayPoint->DeleteWayPoint(m_pClickedObj);
		}

		if (InputKeyPress(E_Key::LBUTTON))
			m_pPressObj = _GetClickedObj(vWorldPos);
		if (InputKeyRelease(E_Key::LBUTTON)) {
			m_pReleaseObj = _GetClickedObj(vWorldPos);
			if (m_pPressObj && !m_pReleaseObj &&
				m_pPressObj == m_pReleaseObj) {
				m_pClickedObj = m_pPressObj;
			}
			m_pPressObj = nullptr;
			m_pReleaseObj = nullptr;
		}
		if (InputKeyHold(E_Key::LBUTTON)) {
			if (m_pPressObj)
				m_pPressObj->Transform()->SetLocalPosition(vWorldPos);
		}

		ImGui::End();
	}
}

void WayPointGUI_bu::_DeletWayPoint(CGameObject* _pWayPointObj)
{
}



bool WayPointGUI_bu::_CheckAndGenerateWayPoint()
{
	if (!m_pWayPointObj) {
		m_pWayPointObj = FIND_GameObject(_T("WayPoint_bu"));
		if (m_pWayPoint) {
			m_pWayPoint = m_pWayPointObj->GetComponent<CWayPoint_bu>();
		}
	}

	if (!m_pWayPointObj) {
		SharedPtr<CPrefab> pPrefab = CResourceManager::GetInstance()->FindRes<CPrefab>(_T("WayPoint_bu"));
		if (pPrefab.Get()) {
			UINT iLayer = (UINT)E_Layer::WayPoint_Tool;
			m_pWayPointObj = CObject::InstantiateEvn(pPrefab, Vector3(0.f, 0.f, -10.f), (UINT)iLayer);
			m_pWayPoint = m_pWayPointObj->GetComponent<CWayPoint_bu>();
		}
		return false;
	}
	if (m_pWayPointObj && !m_pWayPoint) {
		m_pWayPoint = m_pWayPointObj->GetComponent<CWayPoint_bu>();
		if(!m_pWayPoint)
			return false;
	}
	return true;
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
	m_pPressObj = m_pReleaseObj = m_pClickedObj = nullptr;
}
