#pragma once
#include <Engine\CScript.h>
class CMainPanel_bu;
class CInGamePanel_bu;
class CUIContainer_bu : public CScript
{
private:
	CGameObject* m_pMainPanelObj;
	CGameObject* m_pInGamePanelObj;
private:
	CMainPanel_bu* m_pMainPanel;
	CInGamePanel_bu* m_pInGamePanel;

public:
	CMainPanel_bu* GetMainPanel() { return m_pMainPanel; }
	CInGamePanel_bu* GetInGamePanel() { return m_pInGamePanel; }

public:
	virtual void Awake() override;
	virtual void Update() override;

public:
	virtual bool SaveToScene(FILE* _pFile);
	virtual bool LoadFromScene(FILE* _pFile);


public:
	CLONE(CUIContainer_bu);
	CUIContainer_bu();
	virtual ~CUIContainer_bu();
};