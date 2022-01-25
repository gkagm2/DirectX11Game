#pragma once
#include <Engine\CScript.h>
class CLinkTestScript : public CScript
{
public:
	CGameObject* pTargetObj;
public:

	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

	CLONE(CLinkTestScript);
	CLinkTestScript();
	CLinkTestScript(const CLinkTestScript& _origin);
	virtual ~CLinkTestScript() override {}
};