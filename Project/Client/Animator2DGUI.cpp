#include "pch.h"
#include "Animator2DGUI.h"
#include <Engine\CAnimator2D.h>
#include <Engine\CAnimation2D.h>
#include <Engine\CMeshRenderer.h>
#include <Engine\CGameObject.h>
#include "Animator2DEditorGUI.h"
#include "CImGuiManager.h"
#include "ParamGUI.h"
#include <Engine\CCore.h>

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
	if (pAnimation) {
		string strName{};
		TStringToString(pAnimation->GetName().c_str(), strName);
		ImGui::Text("Animation Name : %s", strName.c_str());
	}


	// �ִϸ��̼� ����
	string strAnimState = "";
	TStringToString(AnimationStateToStr(eAnimationState), strAnimState);
	ImGui::Text("State : %s", strAnimState.c_str());

	// �޺��� ǥ���ϱ�
	int iCurItem = (int)eAnimationState;
	ImGui::PushID(CImGuiManager::GetInstance()->GetWidgetID());
	ImGui::Combo("Animation State", &iCurItem, m_strList.data(), (int)m_strList.size());
	ImGui::PopID();
	eAnimationState = (E_AnimationState)iCurItem;

	pAnimator2D->SetAnimationState(eAnimationState);


	// animation list �����ֱ�
	vector<tstring> vectNames;
	GetTargetObject()->Animator2D()->GetAnimationNamesFromList(vectNames);
	vector<string> vecNames_o;
	TStringToStringVec(vectNames, vecNames_o);
	vector<char> vecComboName;
	ParamGUI::Make_ComboBoxList(vecNames_o, vecComboName);
	int iCurAnimIdx = pAnimator2D->GetCurAnimationIdxFromNamesList(vectNames);
	if (ParamGUI::Render_ComboBox("Animation List##animator2D", &iCurAnimIdx, vecComboName)) {
		tstring strAnimName{};
		if (vectNames.size()) {
			tstring strAnimName{};
			if (iCurAnimIdx >= 0) {
				strAnimName = vectNames[iCurAnimIdx];
				pAnimator2D->Play(strAnimName, pAnimator2D->GetAnimationState());
			}
		}
	}


	if (ImGui::Button("Delete Cur Animation##animtor2D")) {
		if (vectNames.size()) {
			tstring strAnimName = vectNames[iCurAnimIdx];
			pAnimator2D->DeleteAnimation(strAnimName);
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("Load Anmiation##Animator2D")) {
		OPENFILENAME ofn;
		wchar_t strMaxPath[MAX_PATH] = L"";
		memset(&ofn, 0, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = CCore::GetInstance()->GetWndHandle();
		ofn.lpstrFilter = _T(".anim");
		ofn.lpstrFile = strMaxPath;
		ofn.nMaxFile = MAX_PATH;

		if (0 != GetOpenFileName(&ofn)) {
			tstring path = ofn.lpstrFile; // ������ ������.
			int find = (int)path.find(STR_DIR_PATH_Anim);
			tstring fileName = path.substr(find, path.size() - find);
			pAnimator2D->LoadAnimation(fileName);
			if (!pAnimator2D->GetCurAnimation()) {
				const auto& iter = pAnimator2D->GetAnimationList().begin();
				if (iter != pAnimator2D->GetAnimationList().end()) {
					tstring strAnimName = iter->first;
					pAnimator2D->Play(strAnimName);
				}
			}
		}
	}

	if (E_AnimationState::Fixed != pAnimator2D->GetAnimationState()) {
		bool bIsPlayOnSceneStart = pAnimator2D->IsPlayOnSceneStart();
		if (ImGui::Checkbox("Start Anim When Scene Play", &bIsPlayOnSceneStart))
			pAnimator2D->PlayOnSceneStart(bIsPlayOnSceneStart);
	}
	else
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
	ImGui::SameLine();
	if (ImGui::Button("Fixed##Animator2D")) {
		Animator2DEditorGUI* pAnimator2DEditorGUI = dynamic_cast<Animator2DEditorGUI*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_Animator2DEditor));
		if (!pAnimator2DEditorGUI)
			assert(nullptr && _T("�ִϸ�����2D �����͸� �� �� ����."));
		else {
			pAnimator2DEditorGUI->SetActive(true);
			pAnimator2DEditorGUI->SetTap
			(Animator2DEditorGUI::E_Tap::Fixed);
			pAnimator2DEditorGUI->SetTargetObject(GetTargetObject());
			pAnimator2DEditorGUI->InitFixedAnimationPanel();
		}
	}

	// select animation
	// show cur anmiation list
	

	// animation play button
	//if (ImGui::Button("Play Animation##animation2D")) {
	//}

	if (pAnimator2D) {
		CAnimation2D* pCurAnim = pAnimator2D->GetCurAnimation();

		if (pCurAnim) {
			const auto& animList = pCurAnim->GetAnimationFrame();
			int iCurAnimFrameIdx = pCurAnim->GetCurFrameIdx();

			if (ImGui::SliderInt("", &iCurAnimFrameIdx, 0, max(0,(int)animList.size() - 1)))
				pAnimation->SetCurAnimationFrame(iCurAnimFrameIdx);
		}

		// PreView Texture�� �־�� �� ��
		TTextureInfo tTexInfo = {};

		
		if (pCurAnim) {
			const TAnimationFrame& curFrame = pCurAnim->GetCurAnimationFrame();

			Vector2 vFinalLT_Vec = curFrame.vLeftTopUV - curFrame.vOffsetPosUV;
			Vector2 vFinalRB_Vec = curFrame.vLeftTopUV + curFrame.vFrameSizeUV - curFrame.vOffsetPosUV;

			tTexInfo.uv_min = ImVec2(vFinalLT_Vec.x, vFinalLT_Vec.y);
			tTexInfo.uv_max = ImVec2(vFinalRB_Vec.x, vFinalRB_Vec.y);
		}

		CTexture* pTex = nullptr;
		if (pCurAnim)
			pTex = pCurAnim->GetCurTexture().Get();
		ParamGUI::Render_Texture("PreView", pTex, nullptr, nullptr, false, tTexInfo);
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