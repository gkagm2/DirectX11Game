#pragma once
#include <Engine\CScript.h>
class CActivatorController : public CScript
{
public:
	CGameObject* m_pTargetActiveObj;

public:
	void Activate();
	void Deactivate();

	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CActivatorController);
	CActivatorController();
	virtual ~CActivatorController() override {}
};