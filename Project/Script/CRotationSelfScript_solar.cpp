#include "pch.h"
#include "CRotationSelfScript_solar.h"

CRotationSelfScript_solar::CRotationSelfScript_solar() :
	CScript((UINT)SCRIPT_TYPE::ROTATIONSELFSCRIPT_SOLAR)
{
}

CRotationSelfScript_solar::~CRotationSelfScript_solar()
{
}

void CRotationSelfScript_solar::Update()
{
	Vector3 vRot = Transform()->GetRotationDegree();
	vRot.y += DT;
	Transform()->SetLocalRotationDegree(vRot);
}