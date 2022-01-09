#include "pch.h"
#include "CWayPoint_bu.h"

CWayPoint_bu::CWayPoint_bu() :
	CScript((UINT)SCRIPT_TYPE::WAYPOINT_BU)
{
	AddParam(TScriptParam{ _T("Way Point Prefab"), E_ScriptParam::PREFAB, m_pWPObjPref.GetAddress()});
}

CWayPoint_bu::~CWayPoint_bu()
{
}
CGameObject* CWayPoint_bu::AddWayPoint(const Vector2& _vPoint)
{
	UINT iLayer = (UINT)E_Layer::WayPoint_Tool;
	Vector3 vWorldPos{ _vPoint.x, _vPoint.y, 0.f };
	assert(m_pWPObjPref.Get());
	CGameObject* pWPObj = CObject::InstantiateEvn(m_pWPObjPref, vWorldPos, iLayer);
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

bool CWayPoint_bu::SaveToScene(FILE* _pFile)
{
	SaveResourceToFile(m_pWPObjPref, _pFile);
	return true;
}

bool CWayPoint_bu::LoadFromScene(FILE* _pFile)
{
	LoadResourceFromFile(m_pWPObjPref, _pFile);
	return true;
}
