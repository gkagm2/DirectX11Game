#pragma once
#include "CInteractiveObj_bu.h"
class CSwitch_bu : public CInteractiveObj_bu
{
private:
	CAnimator2D* m_pAnim;
	bool m_bSwitchState; // 현재 스위치 상태

	// Animation Sprite에 설정할
	int m_iOffIdx;
	int m_iOnIdx;

	CGameObject* m_pTargetObj; //  Interactive 스크립트가 있는 오브젝트

public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;

	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

	CGameObject* GetTargetInteractiveObj() { return m_pTargetObj; }
	void SetTargetInteractiveObj(CGameObject* _pTargetObj) { m_pTargetObj = _pTargetObj; }

public:
	virtual void Interaction(bool _bActive) override;

public:

	CLONE(CSwitch_bu);
	CSwitch_bu();
	CSwitch_bu(const CSwitch_bu& _origin);
	virtual ~CSwitch_bu() override;
};