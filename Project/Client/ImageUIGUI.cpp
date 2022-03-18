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

	// �̹��� ���� ��ư
	CTexture* pImageTexture =  pImageUI->GetImageTex().Get();
	string strKey{};
	if (pImageTexture)
		TStringToString(pImageTexture->GetKey(), strKey);
	
	ParamGUI::Render_Texture(strKey.c_str(), pImageTexture, this, (GUI_CALLBACK)&ImageUIGUI::_SelectHeightMapTexture, true);

	ImGui::Spacing();
	UINT iColor = pImageUI->GetColor();
	if(ParamGUI::Render_Color("Color##ImageUI", &iColor))
		pImageUI->SetColor(iColor);

	// ������Ʈ �߰� ��ư
	//if (ImGui::Button("Add Component##ComponentAdd")) {
	//	// ����Ʈ �並 �����ش�.
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

void ImageUIGUI::_SelectHeightMapTexture(DWORD_PTR _pStr, DWORD_PTR _NONE)
{
	const char* pStrKey = (const char*)_pStr;
	string strKey = pStrKey;
	tstring tStrKey;
	StringToTString(strKey, tStrKey);

	CTexture* pImageTexture = CResourceManager::GetInstance()->FindRes<CTexture>(tStrKey).Get();
	assert(pImageTexture);
	GetTargetObject()->ImageUI()->SetImageTex(pImageTexture);
}