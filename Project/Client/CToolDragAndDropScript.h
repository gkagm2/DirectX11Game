#pragma once
#include <Engine\CScript.h>
class CToolDragAndDropScript : public CScript
{
	bool m_bClick;
private:
	class InspectorGUI* m_pInspGUI;
	class CGameObject* m_pTargetObj;
	Vector3 m_vPrevPos;
	Vector3 m_vCurPos;
	Vector3 m_vPrevTargetPos;
	Vector3 m_vCurTargetPos;


	float m_fDoublePressCoolTime;
	float m_fMaxDoublePressCoolTime;
	int m_iPressCnt;

public:
	virtual void Start() override;
	virtual void Update() override;
public:
	void SetTargetObject(CGameObject* _pTargetObj) { m_pTargetObj = m_pTargetObj; }
	CGameObject* GetTargetObject() { return m_pTargetObj; }
public:
	CLONE_DISABLE(CToolDragAndDropScript);
	CToolDragAndDropScript();
	virtual ~CToolDragAndDropScript() override;
};