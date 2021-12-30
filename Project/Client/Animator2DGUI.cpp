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

	// 현재 애니메이션 이름
	if (pAnimation) {
		string strName{};
		TStringToString(pAnimation->GetName().c_str(), strName);
		ImGui::Text("Animation Name : %s", strName.c_str());
	}
		

	// 애니메이션 상태
	string strAnimState = "";
	TStringToString(AnimationStateToStr(eAnimationState), strAnimState);
	ImGui::Text("State : %s", strAnimState.c_str());

	// 콤보로 표현하기
	static int iCurItem = (int)eAnimationState;
	ImGui::Combo("Animation State", &iCurItem, m_strList.data(), (int)m_strList.size());
	eAnimationState = (E_AnimationState)iCurItem;

	pAnimator2D->SetAnimationState(eAnimationState);

	
	// animation list 보여주기
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
	

	if (ImGui::Button("Delete Cur Animation##animtor2D")) {
		tstring strAnimName = vectNames[iCurAnimIdx];
		pAnimator2D->DeleteAnimation(strAnimName);
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
			tstring path = ofn.lpstrFile; // 파일을 가져옴.
			int find = (int)path.find(STR_DIR_PATH_Anim);
			tstring fileName = path.substr(find, path.size() - find);
			pAnimator2D->LoadAnimation(fileName);
		}
	}

	if (ImGui::Button("Editor Open##Animator2D")) {
		Animator2DEditorGUI* pAnimator2DEditorGUI = dynamic_cast<Animator2DEditorGUI*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_Animator2DEditor));
		if (!pAnimator2DEditorGUI)
			assert(nullptr && _T("애니메이터2D 에디터를 열 수 없다."));
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
		CAnimation2D* pCurAnim = pAnimator2D->GetCurAnimation();

		if (pCurAnim) {
			const auto& animList = pCurAnim->GetAnimationFrame();
			static int iCurAnimFrameIdx = 0;
			if (E_AnimationState::Fixed != pAnimator2D->GetAnimationState())
				iCurAnimFrameIdx = pCurAnim->GetCurFrameIdx();

			if (ImGui::SliderInt("", &iCurAnimFrameIdx, 0, (int)animList.size() - 1))
				pAnimation->SetCurAnimationFrame(iCurAnimFrameIdx);
		}

		// PreView Texture가 있어야 될 듯
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
	// Combo Box의 리스트 생성
	string strState;
	for (UINT i = 0; i < (UINT)E_AnimationState::End; ++i) {
		TStringToString(AnimationStateToStr((E_AnimationState)i), strState);
		for (UINT j = 0; j < strState.size(); ++j)
			m_strList.push_back(strState[j]);
		m_strList.push_back('\0');
	}
	m_strList.push_back('\0');
}