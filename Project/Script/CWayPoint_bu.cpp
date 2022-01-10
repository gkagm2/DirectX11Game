#include "pch.h"
#include "CWayPoint_bu.h"
#include <Engine\CObjectManager.h>

CWayPoint_bu::CWayPoint_bu() :
	CScript((UINT)SCRIPT_TYPE::WAYPOINT_BU)
{
}

CWayPoint_bu::~CWayPoint_bu()
{
}
CGameObject* CWayPoint_bu::AddWayPoint(const Vector2& _vPoint)
{
	UINT iLayer = (UINT)E_Layer::WayPoint_Tool;
	Vector3 vWorldPos{ _vPoint.x, _vPoint.y, 0.f };
	CGameObject* pWPObj = _CreateWayPointObj();
	CGameObject* pParentObj = GetGameObject();
	pWPObj->Transform()->SetLocalPosition(vWorldPos);
	CObject::AddChildGameObjectEvn(pParentObj, pWPObj);
	return pWPObj;
}

void CWayPoint_bu::DeleteWayPoint(CGameObject* _pObj)
{
	auto iter = GetGameObject()->GetChildsObject().begin();
	for (iter; iter != GetGameObject()->GetChildsObject().end(); ++iter) {
		if ((*iter) == _pObj) {
			break;
		}
	}
	if(iter != GetGameObject()->GetChildsObject().end())
		DestroyGameObjectEvn((*iter));
}

vector<CGameObject*>& CWayPoint_bu::GetWayPointObjs() {
	return  GetGameObject()->GetChildsObject();
}

CGameObject* CWayPoint_bu::_CreateWayPointObj()
{
	UINT iLayer = (UINT)E_Layer::WayPoint_Tool;
	CGameObject* pObj = CObjectManager::GetInstance()->CreateEmptyGameObject(iLayer);
	pObj->SetName(BUTCHER_ObjName_WayPointPos_bu);
	pObj->AddComponent<CCollider2D>();
	pObj->Transform()->SetLocalScale(Vector3(0.3f, 0.3f, 1.f));
	return pObj;
}

bool CWayPoint_bu::SaveToScene(FILE* _pFile)
{
	return true;
}

bool CWayPoint_bu::LoadFromScene(FILE* _pFile)
{
	return true;
}
