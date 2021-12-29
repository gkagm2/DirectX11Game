#pragma once
#include <Engine\CScript.h>
class CUIManager_bu : public CScript
{
private:
	static CUIManager_bu* pInst;
public:
	
	CUIManager_bu* GetInstance();
public:
	CLONE(CUIManager_bu);
	CUIManager_bu();
	virtual ~CUIManager_bu();
};
