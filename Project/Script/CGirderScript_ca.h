#pragma once
#include "CModuleScript_ca.h"
class CGirderScript_ca : public CModuleScript_ca
{
public:
	virtual void Start() override;
	virtual void Update() override;

	virtual void OnCollisionEnter2D(CCollider2D* _pOther);
	virtual void OnCollisionStay2D(CCollider2D* _pOther);
	virtual void OnCollisionExit2D(CCollider2D* _pOther);

public:
	virtual void InitModule(E_ModuleLevel_ca _eLevel) override;

public:
	CLONE(CGirderScript_ca);
	CGirderScript_ca();
	CGirderScript_ca(UINT _iScriptNum);
	virtual ~CGirderScript_ca() override;
};