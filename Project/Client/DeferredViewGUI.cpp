#include "pch.h"
#include "DeferredViewGUI.h"
#include "ParamGUI.h"
#include <Engine\CMeshRenderer.h>
#include <Engine\CMaterial.h>
#include <Engine\CResourceManager.h>
#include <Engine\CRenderManager.h>
#include <Engine\CMRT.h>
#include "CImGuiManager.h"
#include "InspectorGUI.h"
#include <Engine\CMeshRenderer.h>
#include <Engine\CGameObject.h>
#include <Engine\CGraphicsShader.h>
#include <Engine\CDecal.h>

DeferredViewGUI::DeferredViewGUI() :
	m_pInspectorGUI(nullptr)
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
				/*ImGui::SameLine();
				ImGui::Text("Depth Stencil");
				ImGui::SameLine();
				SharedPtr<CTexture> pDSTex = pMRT->GetDepthStencilTexture();
				ParamGUI::Render_Texture("", pDSTex.Get(), nullptr, nullptr, false);*/
			}
			ImGui::Separator();
		}
		
		ImGui::Separator();
		ImGui::Separator();


		
		if (!m_pInspectorGUI) {
			m_pInspectorGUI = dynamic_cast<InspectorGUI*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_Inspector));
		}

		if (m_pInspectorGUI) {
			if (E_InspectorUIMode::GameObject == m_pInspectorGUI->GetInspectorUIMode()) {
				CGameObject* pTargetObj = m_pInspectorGUI->GetTargetObject();

				string strObjName = {};
				TStringToString(pTargetObj->GetName(), strObjName);
				ImGui::Text("Object Name : %s", strObjName.c_str());

				if (pTargetObj && 
					pTargetObj->MeshRenderer() && 
					pTargetObj->MeshRenderer()->GetSharedMaterial().Get() &&
					pTargetObj->MeshRenderer()->GetSharedMaterial()->GetShader().Get()) {
					CGraphicsShader* pShader = pTargetObj->MeshRenderer()->GetSharedMaterial()->GetShader().Get();
					if (E_RenderTimePoint::Deferred == pShader->GetRenderTimePoint()) {

						CMaterial* deferredMtrl = pTargetObj->MeshRenderer()->GetSharedMaterial().Get();

						CTexture* pColorTargetTex = nullptr;
						CTexture* pNormalTargetTex = nullptr;
						CTexture* pPositionTargetTex = nullptr;
						CTexture* pDataTargetTex = nullptr;

						deferredMtrl->GetData(E_ShaderParam::Texture_0, &pColorTargetTex);
						deferredMtrl->GetData(E_ShaderParam::Texture_1, &pNormalTargetTex);
						deferredMtrl->GetData(E_ShaderParam::Texture_2, &pPositionTargetTex); 
						deferredMtrl->GetData(E_ShaderParam::Texture_3, &pDataTargetTex);

						ImGui::Text("Color Target Tex");
						ImGui::SameLine();
						ParamGUI::Render_Texture("", pColorTargetTex, nullptr, nullptr, false);
						ImGui::Text("Normal Target Tex");
						ImGui::SameLine();
						ParamGUI::Render_Texture("", pNormalTargetTex, nullptr, nullptr, false);
						ImGui::Text("Position Target Tex");
						ImGui::SameLine();
						ParamGUI::Render_Texture("", pPositionTargetTex, nullptr, nullptr, false);
						ImGui::Text("Data Target Tex");
						ImGui::SameLine();
						ParamGUI::Render_Texture("", pDataTargetTex, nullptr, nullptr, false);


					}
				}
				else if (pTargetObj && pTargetObj->Light3D()) {
					CMaterial* pMaterial = pTargetObj->Light3D()->GetLightMaterial().Get();

					CTexture* pTex = nullptr;
					int iNum[4] = {
						(int)E_ShaderParam::Texture_0,
						(int)E_ShaderParam::Texture_1,
						(int)E_ShaderParam::Texture_2,
						(int)E_ShaderParam::Texture_3
					};

					for (int i = 0; i < 4; ++i) {
						ImGui::Text("Texture %d", i);
						ImGui::SameLine();
						pMaterial->GetData((E_ShaderParam)iNum[i], &pTex);
						ParamGUI::Render_Texture("", pTex, nullptr, nullptr, false);
					}
				}
				else if (pTargetObj && pTargetObj->Decal()) {
					CMaterial* pMaterial = pTargetObj->Decal()->GetMaterial().Get();

					CTexture* pTex = nullptr;
					int iNum[2] = {
						(int)E_ShaderParam::Texture_0,
						(int)E_ShaderParam::Texture_1,
					};

					for (int i = 0; i < 2; ++i) {
						ImGui::Text("Texture %d", i);
						ImGui::SameLine();
						pMaterial->GetData((E_ShaderParam)iNum[i], &pTex);
						ParamGUI::Render_Texture("", pTex, nullptr, nullptr, false);
					}

				}
			}
		}
	}

	ImGui::End();
}