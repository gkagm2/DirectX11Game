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

		CMRT* pMRT = nullptr;
		for (UINT i = 0; i < (UINT)E_MRTType::End; ++i) {
			pMRT = CRenderManager::GetInstance()->GetMultipleRenderTargets((E_MRTType)i);

			ImGui::Separator();

			string strMRTType = {};
			TStringToString(pMRT->GetName(), strMRTType);
			ImGui::Text(strMRTType.c_str());

			ImGui::Spacing();
			SharedPtr<CTexture>* pRTTexs = pMRT->GetRenderTargetTextures();

			for (UINT iTargetIdx = 0; iTargetIdx < pMRT->GetRenderTargetCount(); ++iTargetIdx) {
				string strTexName = {};
				TStringToString(pRTTexs[iTargetIdx]->GetName(), strTexName);
				ImGui::Text(strTexName.c_str());
				ImGui::SameLine();
				ParamGUI::Render_Texture("", pRTTexs[iTargetIdx].Get(), nullptr, nullptr, false);

				SharedPtr<CTexture> pDSTex = pMRT->GetDepthStencilTexture();
				ParamGUI::Render_Texture("Depth Stencil", pDSTex.Get(),nullptr, nullptr, false);
			}
			ImGui::Separator();
		}
		
		ImGui::Separator();

		CTexture* pColorTargetTex = CResourceManager::GetInstance()->FindRes<CTexture>(STR_ResourceKey_Deferred_ColorTargetTex).Get();

		CTexture* pNormalTargetTex = CResourceManager::GetInstance()->FindRes<CTexture>(STR_ResourceKey_Deferred_NormalTargetTex).Get();

		CTexture* pPositionTargetTex = CResourceManager::GetInstance()->FindRes<CTexture>(STR_ResourceKey_Deferred_PositionTargetTex).Get();

		CTexture* pDataTargetTex = CResourceManager::GetInstance()->FindRes<CTexture>(STR_ResourceKey_Deferred_DataTargetTex).Get();

		CMaterial* pMergeMtrl = nullptr;
		pMergeMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_MergeMtrl).Get();

		ImGui::Text("deferred Color Target Tex");
		ImGui::SameLine();
		ParamGUI::Render_Texture("", pColorTargetTex, nullptr, nullptr, false);
		ImGui::Text("deferred Normal Target Tex");
		ImGui::SameLine();
		ParamGUI::Render_Texture("", pNormalTargetTex, nullptr, nullptr, false);
		ImGui::Text("deferred Position Target Tex");
		ImGui::SameLine();
		ParamGUI::Render_Texture("", pPositionTargetTex, nullptr, nullptr, false);
		ImGui::Text("deferred Data Target Tex");
		ImGui::SameLine();
		ParamGUI::Render_Texture("", pDataTargetTex, nullptr, nullptr, false);

		ImGui::Separator();

		CTexture* pMergeColorTargetTex = nullptr;
		CTexture* pMergeDiffuseTargetTex = nullptr;
		CTexture* pMergeSpecularTargetTex = nullptr;
		CTexture* pMergeDataTargetTex = nullptr;
		pMergeMtrl->GetData(E_ShaderParam::Texture_0, &pMergeColorTargetTex);
		pMergeMtrl->GetData(E_ShaderParam::Texture_1, &pMergeDiffuseTargetTex);
		pMergeMtrl->GetData(E_ShaderParam::Texture_2, &pMergeSpecularTargetTex);
		pMergeMtrl->GetData(E_ShaderParam::Texture_4, &pMergeDataTargetTex);

		ImGui::Text("Merge Color Target Tex");
		ImGui::SameLine();
		ParamGUI::Render_Texture("", pMergeColorTargetTex, nullptr, nullptr, false);
		
		ImGui::Text("Merge Diffuse Target Tex");
		ImGui::SameLine();
		ParamGUI::Render_Texture("", pMergeDiffuseTargetTex, nullptr, nullptr, false);
		
		ImGui::Text("Merge Specular Target Tex");
		ImGui::SameLine();
		ParamGUI::Render_Texture("", pMergeSpecularTargetTex, nullptr, nullptr, false);
		
		ImGui::Text("Merge Data Target Tex");
		ImGui::SameLine();
		ParamGUI::Render_Texture("", pMergeDataTargetTex, nullptr, nullptr, false);

		ImGui::Separator();
	}


	ImGui::End();
}