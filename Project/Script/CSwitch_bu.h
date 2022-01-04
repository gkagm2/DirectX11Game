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

public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;

	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	virtual void Interaction(bool _bActive) override;

public:

	CLONE(CSwitch_bu);
	CSwitch_bu();
	virtual ~CSwitch_bu() override;
};