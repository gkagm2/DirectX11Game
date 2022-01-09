#pragma once
#include "GUI.h";
class CGameObject;
class CWayPoint_bu;
class WayPointGUI_bu : public GUI
{
	CGameObject* m_pWayPointObj;
	CWayPoint_bu* m_pWayPoint;

	CGameObject* m_pDragObj;

public:
	virtual void Init() override;
	virtual void Update() override;

protected:
	void _DeletWayPoint(CGameObject* _pWayPointObj);

private:
	CGameObject* _CreateWayPointObj();
	CGameObject* _GetClickedObj(const Vector3& _vWorldPos);

	void _Clear();

public:
	WayPointGUI_bu();
	virtual ~WayPointGUI_bu();
};