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
	Vector3 vForce = pTargetObj->Rigidbody2D()->GetForce();
	

	float fDrag = pTargetObj->Rigidbody2D()->GetDrag();
	if(ImGui::InputFloat("Air Drag  ##Rigidbody2D", &fDrag, 0, 0, "%.2f"))
		pTargetObj->Rigidbody2D()->SetDrag(fDrag);

	float fMass = pTargetObj->Rigidbody2D()->GetMass();
	if(ImGui::InputFloat("Mass  ##Rigidbody2D", &fMass, 0, 0, "%.2f"))
		pTargetObj->Rigidbody2D()->SetMass(fMass);

	float fMaxSpeed = pTargetObj->Rigidbody2D()->GetMaxSpeed();
	if(ImGui::InputFloat("Max Speed##Rigidbody2D", &fMaxSpeed, 0, 0, "%.2f"))
		pTargetObj->Rigidbody2D()->SetMaxSpeed(fMaxSpeed);
	
	bool bGravity = pTargetObj->Rigidbody2D()->IsOnGravity();
	if (ImGui::Checkbox("Use Gravity", &bGravity))
		pTargetObj->Rigidbody2D()->UseGravity(bGravity);


	ImGui::Spacing();
	ImGui::Text("View");
	// 수정 불가능
	float fSpeed = pTargetObj->Rigidbody2D()->GetSpeed();
	ImGui::InputFloat("Cur Speed ##Rigidbody2D", &fSpeed, 0, 0, "%.2f");

	ImGui::Spacing();
	// 수정 가능한데 스크립트로 이용
	float arrForce[3] = { vForce.x, vForce.y, vForce.z };
	ImGui::InputFloat3("Force##Rigidbody2D", arrForce, "%.2f");
	
	Vector3 vVelocity = pTargetObj->Rigidbody2D()->GetVelocity();
	float arrVelocity[3] = { vVelocity.x, vVelocity.y, vVelocity.z };
	if (ImGui::InputFloat3("Velocity##Rigidbody2D", arrVelocity, "%.2f"))
		pTargetObj->Rigidbody2D()->SetVelocity(Vector3(arrVelocity[0], arrVelocity[1], arrVelocity[2]));
	
	

	End();
}