#pragma once
#include <Engine\CScript.h>
class CWayPoint_bu : public CScript
{
private:
	vector<CGameObject*> m_vecWPObjs;
	SharedPtr<CPrefab> m_pWPObjPref; // WP : Way point

public:
	void AddWayPoint(const Vector2& _vWorldPoint);
	void DeleteWayPoint(CGameObject* pObj);

	vector<CGameObject*>& GetWayPointObjs() { return m_vecWPObjs; }

	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CWayPoint_bu);
	CWayPoint_bu();
	virtual ~CWayPoint_bu() override;
};