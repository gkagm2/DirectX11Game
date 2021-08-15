#pragma once
#include <Engine\CScript.h>
class CTestLight2DScript : public CScript
{
private:
	float m_fAngle;
public:
	virtual void Start() override;
	virtual void Update() override;

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CTestLight2DScript);
	CTestLight2DScript();
	virtual ~CTestLight2DScript() override;
};