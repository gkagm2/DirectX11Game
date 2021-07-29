#include "pch.h"
#include "MaterialGUI.h"
#include "ListViewGUI.h"
#include "CImGuiManager.h"

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