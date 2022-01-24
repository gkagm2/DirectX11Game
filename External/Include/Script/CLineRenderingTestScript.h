#pragma once
#include <Engine\CScript.h>
class CLineRenderingTestScript : public CScript
{
private:
	class CLine* m_pLine;
public:
	virtual void Awake();
	virtual void Update();
	virtual void LateUpdate();

public:
	CLONE(CLineRenderingTestScript);
	CLineRenderingTestScript();
	virtual ~CLineRenderingTestScript() override;
};