#include "pch.h"
#include "MaterialGUI.h"
#include "ListViewGUI.h"
#include "CImGuiManager.h"
#include "ParamGUI.h"

#include <Engine\CResourceManager.h>
#include <Engine\CMaterial.h>
#include <Engine\CShader.h>
#include <Engine\CGraphicsShader.h>

MaterialGUI::MaterialGUI() :
	ResourceGUI(E_ResourceType::Material)
{
}

MaterialGUI::~MaterialGUI()
{
}

void MaterialGUI::Update()
{
	if (false == Start())
		return;

	CMaterial* pMtrl = (CMaterial*)GetTargetResource();
	CShader* pShader = pMtrl->GetShader().Get();

	// Material이 참조하는 Shader 출력
	
	char strShaderName[255];
	GetResourceName(pShader, strShaderName, 255);

	// 쉐이더 정보 출력
	ImGui::Text("Shader");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(150);
	ImGui::InputText("##Shader", strShaderName, 255, ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();

	// 쉐이더 버튼을 클릭 시 
	if (ImGui::Button("Select##Shader")) {
		
		ListViewGUI* pListViewGUI = (ListViewGUI*)CImGuiManager::GetInstance()->FindGUI(STR_GUI_ListView);
		// 목록을 전달한다.
		vector<tstring> vecNames;
		CResourceManager::GetInstance()->GetResourceNames(E_ResourceType::GraphicsShader, vecNames);
		tstring strTitle = _T("Shader");
		pListViewGUI->SetList(vecNames, strTitle);
		pListViewGUI->SetDoubleClickCallBack(this, (GUI_CALLBACK)&MaterialGUI::SetShader);
		pListViewGUI->SetActive(true);
	}

	// 참조하고 있는 쉐이더의 파라미터 목록을 가져온다.
	if (nullptr != pShader) {
		const vector<TShaderParam>& vecParam = pShader->GetShaderParams();
		for (UINT i = 0; i < vecParam.size(); ++i) {
			string strName;
			TStringToString(vecParam[i].strName, strName);

			strName = strName + std::to_string((DWORD_PTR)pShader);
			/*
			char szBuffer[255] = {};
			sprintf(szBuffer, "##%lld", (DWORD_PTR)pShader);
			*/

			switch (vecParam[i].eType) {
			case E_ShaderParam::Int_0:
			case E_ShaderParam::Int_1:
			case E_ShaderParam::Int_2:
			case E_ShaderParam::Int_3: {
				int iTemp = 0;
				pMtrl->GetData(vecParam[i].eType, &iTemp);
				ParamGUI::Render_Int(strName, &iTemp);
				pMtrl->SetData(vecParam[i].eType, &iTemp);
			}
				break;

			case E_ShaderParam::Float_0:
			case E_ShaderParam::Float_1:
			case E_ShaderParam::Float_2:
			case E_ShaderParam::Float_3: {
				float iTemp = 0;
				pMtrl->GetData(vecParam[i].eType, &iTemp);
				ParamGUI::Render_Float(strName, &iTemp);
				pMtrl->SetData(vecParam[i].eType, &iTemp);
			}
				break;

			case E_ShaderParam::Vector2_0:
			case E_ShaderParam::Vector2_1:
			case E_ShaderParam::Vector2_2:
			case E_ShaderParam::Vector2_3:
				break;

			case E_ShaderParam::Vector4_0:
			case E_ShaderParam::Vector4_1:
			case E_ShaderParam::Vector4_2:
			case E_ShaderParam::Vector4_3:
				break;

			case E_ShaderParam::Matrix_0:
			case E_ShaderParam::Matrix_1:
			case E_ShaderParam::Matrix_2:
			case E_ShaderParam::Matrix_3:
				break;

			case E_ShaderParam::Texture_0:
			case E_ShaderParam::Texture_1:
			case E_ShaderParam::Texture_2:
			case E_ShaderParam::Texture_3:
			case E_ShaderParam::Texture_4:
			case E_ShaderParam::Texture_5:
			case E_ShaderParam::Texture_6:
			case E_ShaderParam::Texture_7:
			case E_ShaderParam::TextureArr_0:
			case E_ShaderParam::TextureArr_1:
			case E_ShaderParam::TextureCube_0:
			case E_ShaderParam::TextureCube_1:
				break;
			default:
				assert(nullptr);
				break;
			}



		}
	}

	End();
}

void MaterialGUI::SetShader(DWORD_PTR _pShaderName, DWORD_PTR _NONE)
{
	CMaterial* pMtrl = (CMaterial*)GetTargetResource();

	string strKey = (const char*)_pShaderName;
	tstring tstrKey;
	StringToTString(strKey, tstrKey);
	SharedPtr<CGraphicsShader> pShader = CResourceManager::GetInstance()->FindRes<CGraphicsShader>(tstrKey);

	pMtrl->SetShader(pShader);
}