#include "pch.h"
#include "Animator2DGUI.h"
#include <Engine\CAnimator2D.h>
#include <Engine\CAnimation2D.h>

Animator2DGUI::Animator2DGUI() :
	AnimatorGUI(E_ComponentType::Animator2D)
{
	_InitComboBox();
}

Animator2DGUI::~Animator2DGUI()
{
}


void Animator2DGUI::Update()
{
	if (false == Start())
		return;

	CGameObject* pObj = GetTargetObject();
	CAnimator2D* pAnimator = pObj->Animator2D();

	E_AnimationState eAnimationState = pAnimator->GetAnimationState();
	CAnimation2D* pAnimation = pAnimator->GetCurAnimation();

	// ���� �ִϸ��̼� �̸�
	string strAnimName;
	TStringToString(pAnimation->GetName(), strAnimName);
	ImGui::Text("%s", strAnimName.c_str());

	// �ִϸ��̼� ����
	string strAnimState;
	TStringToString(AnimationStateToStr(eAnimationState), strAnimState);
	ImGui::Text("State : %s", strAnimState.c_str());

	// �޺��� ǥ���ϱ�
	static int iCurItem = (int)eAnimationState;
	ImGui::Combo("Animation State", &iCurItem, m_strList.data(), m_strList.size());
	eAnimationState = (E_AnimationState)iCurItem;

	pAnimator->SetAnimationState(eAnimationState);

	End();
}

void Animator2DGUI::_InitComboBox()
{
	// Combo Box�� ����Ʈ ����
	string strState;
	for (UINT i = 0; i < (UINT)E_AnimationState::End; ++i) {
		TStringToString(AnimationStateToStr((E_AnimationState)i), strState);
		for (UINT j = 0; j < strState.size(); ++j)
			m_strList.push_back(strState[j]);
		m_strList.push_back('\0');
	}
	m_strList.push_back('\0');
}