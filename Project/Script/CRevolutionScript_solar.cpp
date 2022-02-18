#include "pch.h"
#include "CRevolutionScript_solar.h"

CRevolutionScript_solar::CRevolutionScript_solar() :
	CScript((UINT)SCRIPT_TYPE::REVOLUTIONSCRIPT_SOLAR)
{
}

CRevolutionScript_solar::~CRevolutionScript_solar()
{
}

void CRevolutionScript_solar::Update()
{
	if (GetGameObject()->GetParentObject()) {
		Vector3 vRot = GetGameObject()->GetParentObject()->Transform()->GetLocalRotationDegree();
		vRot.y += 20.f * DT;
		GetGameObject()->GetParentObject()->Transform()->SetLocalRotationDegree(vRot);
	}
	// TODO (Jang0 : RotateAround �Լ� ���� �Ϸ� �� �̰ɷ� ��ü�ϱ�
	//Transform()->RotateAround(Vector3(0.f, 0.f, 0.f), Vector3::Up, 30.f * DT);
}