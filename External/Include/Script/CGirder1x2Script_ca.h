#pragma once
#include "CGirderScript_ca.h"
class CGirder1x2Script_ca : public CGirderScript_ca
{
public:
	virtual void Start() override;
	virtual void Update() override;

	virtual void OnCollisionEnter2D(CCollider2D* _pOther);
	virtual void OnCollisionStay2D(CCollider2D* _pOther);
	virtual void OnCollisionExit2D(CCollider2D* _pOther);

public:
	CGirder1x2Script_ca();
	virtual ~CGirder1x2Script_ca() override;
};