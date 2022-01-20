#pragma once
#include "CInteractiveObj_bu.h"
class CSwitch_bu : public CInteractiveObj_bu
{
private:
	CAnimator2D* m_pAnim;
	bool m_bSwitchState; // ���� ����ġ ����

	// Animation Sprite�� ������
	int m_iOffIdx;
	int m_iOnIdx;

	CGameObject* m_pTargetObj; //  Interactive ��ũ��Ʈ�� �ִ� ������Ʈ
	CLight2D* m_pLight;

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