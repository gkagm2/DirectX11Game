#include "pch.h"
#include "ImageUIGUI.h"
#include <Engine\CImageUI.h>
#include "ListViewGUI.h"
#include "CImGuiManager.h"
#include <Engine\CResourceManager.h>
#include "ParamGUI.h"

ImageUIGUI::ImageUIGUI() :
	ComponentGUI(E_ComponentType::ImageUI)
{
}

ImageUIGUI::~ImageUIGUI()
{
}

void ImageUIGUI::Update()
{
	if (false == Start())
		return;

	CImageUI* pImageUI = GetTargetObject()->ImageUI();

	// 이미지 변경 버튼
	ImGui::Text("Texture ");
	if (ImGui::Button("Change")) {
		ListViewGUI* pListView = dynamic_cast<ListViewGUI*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_ListView));
		assert(pListView);
		if (pListView) {
			// Texture Names
			vector<tstring> vecNames;
			CResourceManager::GetInstance()->GetResourceKeys(E_ResourceType::Texture, vecNames);
			pListView->SetList(vecNames, ResourceTypeToStr(E_ResourceType::Texture));
			pListView->SetDoubleClickCallBack(this, (GUI_CALLBACK)&ImageUIGUI::SelectTexture);
			pListView->SetActive(true);
		}
	}

	CTexture* pImageTexture =  pImageUI->GetImageTex().Get();
	if (pImageTexture) {
		string strKey;
		TStringToString(pImageTexture->GetKey().c_str(), strKey);
		if (ParamGUI::Render_Texture(strKey.c_str(), pImageTexture, nullptr, nullptr, false)) {
		}
	}

	ImGui::Text("Color");


	// 컴포넌트 추가 버튼
	//if (ImGui::Button("Add Component##ComponentAdd")) {
	//	// 리스트 뷰를 보여준다.
	//	ListViewGUI* pListViewGUI = dynamic_cast<ListViewGUI*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_ListView));
	//	assert(pListViewGUI);

	//	vector<tstring> vecNames;
	//	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
	//		CComponent* pComponent = m_pTargetObject->GetComponent((E_ComponentType)i);
	//		if (pComponent)
	//			continue;

	//		tstring strComponentName = ComponentTypeToStr((E_ComponentType)i);
	//		vecNames.push_back(strComponentName);
	//	}

	//	pListViewGUI->SetList(vecNames, _T("Component"));
	//	pListViewGUI->SetDoubleClickCallBack(this, (GUI_CALLBACK)&InspectorGUI::_AddNewComponent);
	//	pListViewGUI->SetActive(true);
	//}


	End();
}

void ImageUIGUI::SelectTexture(DWORD_PTR _pStr, DWORD_PTR _NONE)
{
	const char* pStrKey = (const char*)_pStr;
	string strKey = pStrKey;
	tstring tStrKey;
	StringToTString(strKey, tStrKey);

	CTexture* pImageTexture = CResourceManager::GetInstance()->FindRes<CTexture>(tStrKey).Get();
	assert(pImageTexture);
	GetTargetObject()->ImageUI()->SetImageTex(pImageTexture);
}