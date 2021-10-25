#pragma once
#include <Engine\CScript.h>
class CCollider2D;
class CColliderTestScript : public CScript
{
public:
	virtual void Update() override;

public:
	virtual void OnCollisionEnter2D(CCollider2D* _pOther);
	virtual void OnCollisionStay2D(CCollider2D* _pOther);
	virtual void OnCollisionExit2D(CCollider2D* _pOther);



public:
	CLONE(CColliderTestScript);
	CColliderTestScript();
	virtual ~CColliderTestScript() override;
};