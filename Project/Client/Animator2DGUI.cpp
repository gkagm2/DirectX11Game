#include "pch.h"
#include "Animator2DGUI.h"
#include <Engine\CAnimator2D.h>
#include <Engine\CAnimation2D.h>
#include <Engine\CMeshRenderer.h>
#include <Engine\CGameObject.h>
#include "Animator2DEditorGUI.h"
#include "CImGuiManager.h"
#include "ParamGUI.h"

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
	CAnimator2D* pAnimator2D = pObj->Animator2D();

	E_AnimationState eAnimationState = pAnimator2D->GetAnimationState();
	CAnimation2D* pAnimation = pAnimator2D->GetCurAnimation();

	// ���� �ִϸ��̼� �̸�
	if (pAnimation)
		ImGui::Text("Animation Name : %s", pAnimation->GetName().c_str());

	// �ִϸ��̼� ����
	string strAnimState = "";
	TStringToString(AnimationStateToStr(eAnimationState), strAnimState);
	ImGui::Text("State : %s", strAnimState.c_str());

	// �޺��� ǥ���ϱ�
	static int iCurItem = (int)eAnimationState;
	ImGui::Combo("Animation State", &iCurItem, m_strList.data(), m_strList.size());
	eAnimationState = (E_AnimationState)iCurItem;

	pAnimator2D->SetAnimationState(eAnimationState);

	
	// animation list �����ֱ�
	vector<tstring> vectNames;
	GetTargetObject()->Animator2D()->GetAnimationNamesFromList(vectNames);
	vector<string> vecNames_o;
	TStringToStringVec(vectNames, vecNames_o);
	vector<char> vecComboName;
	ParamGUI::Make_ComboBoxList(vecNames_o, vecComboName);
	static int iCurAnimIdx = 0;
	if (ParamGUI::Render_ComboBox("Animation List##animator2D", &iCurAnimIdx, vecComboName)) {
		tstring strAnimName = vectNames[iCurAnimIdx];
		pAnimator2D->Play(strAnimName, pAnimator2D->GetAnimationState());
	}


	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	

	if (ImGui::Button("Editor Open##Animator2D")) {
		Animator2DEditorGUI* pAnimator2DEditorGUI = dynamic_cast<Animator2DEditorGUI*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_Animator2DEditor));
		if (!pAnimator2DEditorGUI)
			assert(nullptr && _T("�ִϸ�����2D �����͸� �� �� ����."));
		else {
			pAnimator2DEditorGUI->SetActive(true);
			pAnimator2DEditorGUI->SetTargetObject(GetTargetObject());
		}
	}

	// select animation
	// show cur anmiation list
	

	// animation play button
	//if (ImGui::Button("Play Animation##animation2D")) {
	//}

	if (pAnimator2D) {
		// PreView Texture�� �־�� �� ��
		TTextureInfo tTexInfo = {};

		CAnimation2D* pCurAnim = pAnimator2D->GetCurAnimation();
		if (pCurAnim) {
			const TAnimationFrame& curFrame = pCurAnim->GetCurAnimationFrame();

			Vector2 vFinalLT_Vec = curFrame.vLeftTopUV - curFrame.vOffsetPosUV;
			Vector2 vFinalRB_Vec = curFrame.vLeftTopUV + curFrame.vFrameSizeUV - curFrame.vOffsetPosUV;

			tTexInfo.uv_min = ImVec2(vFinalLT_Vec.x, vFinalLT_Vec.y);
			tTexInfo.uv_max = ImVec2(vFinalRB_Vec.x, vFinalRB_Vec.y);


			ParamGUI::Render_Texture("PreView", pAnimator2D->GetCurAnimation()->GetCurTexture().Get(), nullptr, nullptr, false, tTexInfo);
		}
	}

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