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
void CWayPoint_bu::AddWayPoint(const Vector2& _vPoint)
{
	UINT iLayer = (UINT)E_Layer::WayPoint_Tool;
	Vector3 vWorldPos{ _vPoint.x, _vPoint.y, 0.f };
	assert(m_pWPObjPref.Get());
	CGameObject* pWPObj = CObject::InstantiateEvn(m_pWPObjPref, vWorldPos, iLayer);
	m_vecWPObjs.push_back(pWPObj);
	CObject::AddChildGameObjectEvn(GetGameObject(), pWPObj);
	pWPObj->Transform()->SetLocalPosition(vWorldPos);
}

void CWayPoint_bu::DeleteWayPoint(CGameObject* _pObj)
{
	auto iter = m_vecWPObjs.begin();
	for (iter; iter != m_vecWPObjs.end(); ++iter) {
		if ((*iter) == _pObj) {
			break;
		}
	}

	if (iter != m_vecWPObjs.end())
		m_vecWPObjs.erase(iter);
	DestroyGameObjectEvn(_pObj);
}

bool CWayPoint_bu::SaveToScene(FILE* _pFile)
{
	SaveResourceToFile(m_pWPObjPref, _pFile);
	size_t iCnt = m_vecWPObjs.size();
	FWrite(iCnt, _pFile);
	for (size_t i = 0; i < m_vecWPObjs.size(); ++i) {
		Vector3 vLocalPos = m_vecWPObjs[i]->Transform()->GetLocalPosition();
		FWrite(vLocalPos, _pFile);
	}
	return true;
}

bool CWayPoint_bu::LoadFromScene(FILE* _pFile)
{
	LoadResourceFromFile(m_pWPObjPref, _pFile);
	size_t iCnt = 0;
	FRead(iCnt, _pFile);
	for (size_t i = 0; i < iCnt; ++i) {
		Vector3 vLocalPos{};
		FRead(vLocalPos, _pFile);
		AddWayPoint(vLocalPos.XY());
	}
	return true;
}
