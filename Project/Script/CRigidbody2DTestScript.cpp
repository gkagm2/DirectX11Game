#include "pch.h"
#include "CRigidbody2DTestScript.h"

CRigidbody2DTestScript::CRigidbody2DTestScript() :
	CScript((UINT)SCRIPT_TYPE::RIGIDBODY2DTESTSCRIPT)
{
	AddParam(TScriptParam{ _T("Speed"), E_ScriptParam::FLOAT, &fSpeed });
}


void CRigidbody2DTestScript::Start()
{
}

void CRigidbody2DTestScript::Update()
{
	Vector3 dir = {};
	bool inputKey = false;
	if (InputKeyHold(E_Key::UP)) {
		dir += Vector3(0,1,0);
		inputKey = true;
	}
	if (InputKeyHold(E_Key::LEFT)) {
		dir += Vector3(1,0,0);
		inputKey = true;
	}
	if (InputKeyHold(E_Key::RIGHT)) {
		dir += Vector3(-1,0,0);
		inputKey = true;
	}
	if (InputKeyHold(E_Key::DOWN)) {
		dir += Vector3(0,-1,0);
		inputKey = true;
	}
	dir.Normalize();

	if (InputKeyPress(E_Key::NUM1)) {
		deltaType = false;
	}
	if (InputKeyPress(E_Key::NUM2)) {
		deltaType = true;
	}

	if (inputKey) {
		if (deltaType)
			Rigidbody2D()->AddForce(dir * fSpeed * DT);
		else
			Rigidbody2D()->AddForce(dir * fSpeed);
	}
}