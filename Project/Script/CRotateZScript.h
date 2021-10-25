#pragma once
#include <Engine\CScript.h>
class CRotateZScript : public CScript
{
private:
	float m_fSpeed;
public:
	virtual void Update() override;

	virtual void OnCollisionEnter2D(CCollider2D* _pCol) override;
	virtual void OnCollisionStay2D(CCollider2D* _pCol) override;
	virtual void OnCollisionExit2D(CCollider2D* _pCol) override;


public:
	CLONE(CRotateZScript);
	CRotateZScript();
	virtual ~CRotateZScript() override;
};