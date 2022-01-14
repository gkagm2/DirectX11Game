#pragma once
#include <Engine\CScript.h>
class CTargetLookAt_bu : public CScript
{
private:
	CGameObject* m_pTargetObj;
	class CCharacter_bu* m_pCharacter;

public:
	virtual void Awake() override;
	virtual void Update() override;

	void SetTargetObj(CGameObject* _pTargetObj) { m_pTargetObj = _pTargetObj; }
	CGameObject* GetTargetObj() { return m_pTargetObj; }

public:
	CLONE(CTargetLookAt_bu);
	CTargetLookAt_bu();
	virtual ~CTargetLookAt_bu() override {}
};