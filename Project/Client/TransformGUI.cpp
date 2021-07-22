#include "pch.h"
#include "TransformGUI.h"

#include <Engine\CTransform.h>

TransformGUI::TransformGUI() :
	ComponentGUI(E_ComponentType::Transform)
{
}

TransformGUI::~TransformGUI()
{
}

void TransformGUI::Update()
{
	if (false == Start())
		return;

	CTransform* pTransform = GetTargetObject()->Transform();

	Vector3 vTrans = pTransform->GetLocalPosition();
	Vector3 vScale = pTransform->GetLocalScale();
	Vector3 vRotation = pTransform->GetLocalRotation();
	vRotation = vRotation * CMyMath::Deg2Rad();
	pTransform->SetLocalRotation(vRotation);


	End();
}