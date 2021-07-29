#include "pch.h"
#include "Rigidbody2DGUI.h"
#include <Engine\CRigidbody2D.h>

Rigidbody2DGUI::Rigidbody2DGUI() :
	RigidbodyGUI(E_ComponentType::Rigidbody2D)
{
}

Rigidbody2DGUI::~Rigidbody2DGUI()
{
}

void Rigidbody2DGUI::Update()
{
	if (false == Start())
		return;

	CGameObject* pTargetObj = GetTargetObject();
	float fDrag = pTargetObj->Rigidbody2D()->GetDrag();
	float fMass = pTargetObj->Rigidbody2D()->GetMass();
	float fMaxSpeed = pTargetObj->Rigidbody2D()->GetMaxSpeed();
	Vector3 vVelocity = pTargetObj->Rigidbody2D()->GetVelocity();

	Vector3 vForce = pTargetObj->Rigidbody2D()->GetForce();
	float fSpeed = pTargetObj->Rigidbody2D()->GetSpeed();

	ImGui::InputFloat("Drag  ##Rigidbody2D", &fDrag, 0, 0, "%.2f");
	ImGui::InputFloat("Mass  ##Rigidbody2D", &fMass, 0, 0, "%.2f");
	ImGui::InputFloat("Cur Speed ##Rigidbody2D", &fSpeed, 0, 0, "%.2f");
	ImGui::InputFloat("Max Speed##Rigidbody2D", &fMaxSpeed, 0, 0, "%.2f");
	float arrVelocity[3] = { vVelocity.x, vVelocity.y, vVelocity.z };
	ImGui::InputFloat3("Velocity##Rigidbody2D", arrVelocity, "%.2f");
	float arrForce[3] = { vForce.x, vForce.y, vForce.z };
	ImGui::InputFloat3("Force##Rigidbody2D", arrForce, "%.2f");



	pTargetObj->Rigidbody2D()->SetDrag(fDrag);
	pTargetObj->Rigidbody2D()->SetMass(fMass);
	
	pTargetObj->Rigidbody2D()->SetMaxSpeed(fMaxSpeed);
	pTargetObj->Rigidbody2D()->SetVelocity(Vector3(arrVelocity[0], arrVelocity[1], arrVelocity[2]));

	End();
}