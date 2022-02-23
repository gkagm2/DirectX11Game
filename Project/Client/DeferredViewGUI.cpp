#include "pch.h"
#include "DeferredViewGUI.h"
#include "ParamGUI.h"
#include <Engine\CMeshRenderer.h>
#include <Engine\CMaterial.h>
#include <Engine\CResourceManager.h>
#include <Engine\CRenderManager.h>
#include <Engine\CMRT.h>

DeferredViewGUI::DeferredViewGUI()
{
}

DeferredViewGUI::~DeferredViewGUI()
{
}

void DeferredViewGUI::Init()
{
	SetName(STR_GUI_DeferredView);
	SetActive(false);
}

void DeferredViewGUI::Update()
{
	if (!m_bGUIOpen)
		return;

	ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
	if (ImGui::Begin(STR_GUI_DeferredView, &m_bGUIOpen)) {

//#define STR_ResourceKey_Deferred_ColorTargetTex _T("ColorTargetTex")
//#define STR_ResourceKey_Deferred_NormalTargetTex _T("NormalTargetTex")
//#define STR_ResourceKey_Deferred_PositionTargetTex _T("PositionTargetTex")
//#define STR_ResourceKey_Deferred_DataTargetTex _T("DataTargetTex")

		CTexture* pColorTargetTex = nullptr;
		CTexture* pNormalTargetTex = nullptr;
		CTexture* pPositionTargetTex = nullptr;
		CTexture* pDataTargetTex = nullptr;


		CMRT* arrMRT = CRenderManager::GetInstance()->GetMultipleRenderTargets(E_MRTType::Deferred);

		// 모든 MRT를 돌려본다.
		for (UINT i = 0; i < (UINT)E_MRTType::End; ++i) {
			CMRT* pMRT = CRenderManager::GetInstance()->GetMultipleRenderTargets((E_MRTType)i);

			string strMRTType = {};
			TStringToString(pMRT->GetName(), strMRTType);
			ImGui::Text(strMRTType.c_str());


			for (UINT iTargetIdx = 0; iTargetIdx < pMRT->GetRenderTargetCount(); ++iTargetIdx) {
				// TODO (Jang) : 여기부터
			}
		}


		pColorTargetTex = CResourceManager::GetInstance()->FindRes<CTexture>(STR_ResourceKey_Deferred_ColorTargetTex).Get();

		pNormalTargetTex = CResourceManager::GetInstance()->FindRes<CTexture>(STR_ResourceKey_Deferred_NormalTargetTex).Get();

		pPositionTargetTex = CResourceManager::GetInstance()->FindRes<CTexture>(STR_ResourceKey_Deferred_PositionTargetTex).Get();

		pDataTargetTex = CResourceManager::GetInstance()->FindRes<CTexture>(STR_ResourceKey_Deferred_DataTargetTex).Get();

		CMaterial* pMergeMtrl = nullptr;
		pMergeMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_MergeMtrl).Get();

		
		ParamGUI::Render_Texture("deferred Color Target Tex", pColorTargetTex, nullptr, nullptr, false);
		ImGui::SameLine();
		ParamGUI::Render_Texture("deferred Normal Target Tex", pNormalTargetTex, nullptr, nullptr, false);
		ImGui::SameLine();
		ParamGUI::Render_Texture("deferred Position Target Tex", pPositionTargetTex, nullptr, nullptr, false);
		ImGui::SameLine();
		ParamGUI::Render_Texture("deferred Data Target Tex", pDataTargetTex, nullptr, nullptr, false);

		ImGui::Separator();
		ImGui::Separator();

		CTexture* pMergeColorTargetTex = nullptr;
		CTexture* pMergeDiffuseTargetTex = nullptr;
		CTexture* pMergeSpecularTargetTex = nullptr;
		CTexture* pMergeDataTargetTex = nullptr;
		pMergeMtrl->GetData(E_ShaderParam::Texture_0, &pMergeColorTargetTex);
		pMergeMtrl->GetData(E_ShaderParam::Texture_1, &pMergeDiffuseTargetTex);
		pMergeMtrl->GetData(E_ShaderParam::Texture_2, &pMergeSpecularTargetTex);
		pMergeMtrl->GetData(E_ShaderParam::Texture_4, &pMergeDataTargetTex);

		ParamGUI::Render_Texture("Merge Color Target Tex", pMergeColorTargetTex, nullptr, nullptr, false);
		ImGui::SameLine();
		ParamGUI::Render_Texture("Merge Diffuse Target Tex", pMergeDiffuseTargetTex, nullptr, nullptr, false);
		ImGui::SameLine();
		ParamGUI::Render_Texture("Merge Specular Target Tex", pMergeSpecularTargetTex, nullptr, nullptr, false);
		ImGui::SameLine();
		ParamGUI::Render_Texture("Merge Data Target Tex", pMergeDataTargetTex, nullptr, nullptr, false);

		ImGui::Separator();
		ImGui::Separator();
	}


	ImGui::End();
}