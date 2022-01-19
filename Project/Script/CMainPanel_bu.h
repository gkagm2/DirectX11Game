#pragma once
#include <Engine\CScript.h>
class CMainPanel_bu : public CScript
{
private:
	CGameObject* m_pStartBtnObj;
	CGameObject* m_pOptionBtnObj;

private:
	CButtonUI* m_pStartBtn;
	CButtonUI* m_pOptionBtn;

public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;

public:
	void OnStartBtnClick();
	void OnOptionBtnClick();
public:
	CLONE(CMainPanel_bu);
	CMainPanel_bu();
	virtual ~CMainPanel_bu() override;
};