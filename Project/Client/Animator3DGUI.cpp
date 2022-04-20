#include "pch.h"
#include "Animator3DGUI.h"
#include <Engine\CAnimator3D.h>
#include <Engine\CStructuredBuffer.h>

Animator3DGUI::Animator3DGUI() :
	AnimatorGUI(E_ComponentType::Animator3D)
{
	_InitComboBox();
}

Animator3DGUI::~Animator3DGUI()
{
}

void Animator3DGUI::Update()
{
	if (false == Start())
		return;

	CAnimator3D* pAnimator = GetTargetObject()->Animator3D();
	if (nullptr == pAnimator) {
		End();
		return;
	}


	ImGui::Text("Bone Count [%d]", (int)pAnimator->GetBoneCount());

	pAnimator->GetCurAnimation();

	// 특정 프레임의 최종행렬
	CStructuredBuffer* pBuffer =  pAnimator->GetFinalBoneMat();


	// 클립의 현재 프레임
	pAnimator->GetCurFrame();


	End();
}

void Animator3DGUI::_InitComboBox()
{
}
