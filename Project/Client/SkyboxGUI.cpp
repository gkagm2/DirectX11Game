#include "pch.h"
#include "SkyboxGUI.h"
#include <Engine\CSkybox.h>
#include <Engine\CResourceManager.h>
#include <Engine\CMeshRenderer.h>
#include "ParamGUI.h"
#include "CImGuiManager.h"

SkyboxGUI::SkyboxGUI() :
	ComponentGUI(E_ComponentType::Skybox)
{
	for (UINT i = 0; i < (UINT)E_SkyboxType::End; ++i) {
		string str;
		TStringToString(SkyboxTypeToStr((E_SkyboxType)i), str);
		m_vecSkyboxType.push_back(str);
	}
}

SkyboxGUI::~SkyboxGUI()
{
}

void SkyboxGUI::Update()
{
	if (false == Start())
		return;

	
	CSkybox* pSkybox = GetTargetObject()->Skybox();
	
	E_SkyboxType eSkyboxType = pSkybox->GetCurSkyboxType();
	int iCurType = (int)eSkyboxType;
	if(ParamGUI::Render_ComboBox("type##Skybox Type", &iCurType, m_vecSkyboxType)) {
		pSkybox->SetSkyboxType((E_SkyboxType)iCurType);
	}

	if (ImGui::Button("Skybox Texture Select")) {
		// 목록 전달
		vector<tstring> vecName;
		CResourceManager::GetInstance()->GetResourceKeys(E_ResourceType::Texture, vecName);
		ListViewGUI* pListGUI = dynamic_cast<ListViewGUI*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_ListView));
		assert(pListGUI);
		if (pListGUI) {
			pListGUI->SetList(vecName, _T("Skybox Texture"));
			pListGUI->SetDoubleClickCallBack(this, (GUI_CALLBACK)&SkyboxGUI::_SelectTexture);
			pListGUI->SetPreViewTexRender(true);
			pListGUI->SetActive(true);
		}
	}


	End();
}

void SkyboxGUI::_SetMesh(DWORD_PTR _dw1, DWORD_PTR _dw)
{
	string strKey = (const char*)_dw1;
	tstring tstrKey;
	StringToTString(strKey, tstrKey);
	SharedPtr<CMesh> pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(tstrKey);
	assert(pMesh.Get());
	GetTargetObject()->MeshRenderer()->SetMesh(pMesh);
}

void SkyboxGUI::_SetMatrial(DWORD_PTR _dw1, DWORD_PTR _dw)
{
	string strKey = (const char*)_dw1;
	tstring tstrKey;
	StringToTString(strKey, tstrKey);
	SharedPtr<CMaterial> pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(tstrKey);
	assert(pMtrl.Get());
	GetTargetObject()->MeshRenderer()->SetMaterial(pMtrl);
}

void SkyboxGUI::_SelectTexture(DWORD_PTR _strKey, DWORD_PTR _NONE)
{
	// 선택한 텍스쳐를 알아낸다.
	const char* pStrKey = (const char*)_strKey;
	string strKey = pStrKey;
	tstring tStrKey;
	StringToTString(strKey, tStrKey);

	CTexture* pTex = CResourceManager::GetInstance()->FindRes<CTexture>(tStrKey).Get();
	assert(pTex);
	GetTargetObject()->Skybox()->SetSkyboxTexture(pTex);
}