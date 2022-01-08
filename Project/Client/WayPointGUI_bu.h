#pragma once
#include "GUI.h";
class CGameObject;
class CWayPoint_bu;
class WayPointGUI_bu : public GUI
{
	CGameObject* m_pWayPointObj;
	CWayPoint_bu* m_pWayPoint;

	CGameObject* m_pPressObj;
	CGameObject* m_pReleaseObj;
	CGameObject* m_pClickedObj;

public:
	virtual void Init() override;
	virtual void Update() override;

protected:
	void _DeletWayPoint(CGameObject* _pWayPointObj);

private:
	bool _CheckAndGenerateWayPoint();
	CGameObject* _GetClickedObj(const Vector3& _vWorldPos);

	void _Clear();

public:
	WayPointGUI_bu();
	virtual ~WayPointGUI_bu();
};