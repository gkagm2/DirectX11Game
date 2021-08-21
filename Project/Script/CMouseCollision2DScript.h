#pragma once
#include <Engine\CScript.h>
class CMouseCollision2DScript : public CScript
{
private:
	list<CGameObject*> m_listHitObjs;

	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;

public:
	CLONE(CMouseCollision2DScript);
	CMouseCollision2DScript();
	virtual ~CMouseCollision2DScript() override;
};