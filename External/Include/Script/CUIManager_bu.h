#pragma once
#include <Engine\CScript.h>

class CUIContainer_bu;
class CUIManager_bu : public CScript
{
	SINGLETON_SCRIPT(CUIManager_bu);

private:
	CUIContainer_bu* m_pContainer;

public:
	virtual void Awake() override;
public:
	CUIContainer_bu* GetContainer() { return m_pContainer; }
public:
	CLONE(CUIManager_bu);
};
