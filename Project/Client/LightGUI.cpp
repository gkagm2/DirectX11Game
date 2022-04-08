#include "pch.h"
#include "LightGUI.h"
#include <Engine\CResourceManager.h>
#include <Engine\CMeshRenderer.h>

void LightGUI::_InitComboBoxList()
{
	string strState;
	for (UINT i = 0; i < (UINT)E_LightType::End; ++i) {
		TStringToString(LightTypeToStr((E_LightType)i), strState);
		for (UINT j = 0; j < strState.size(); ++j)
			m_vecStrLigthTypeList.push_back(strState[j]);
		m_vecStrLigthTypeList.push_back('\0');
	}
	m_vecStrLigthTypeList.push_back('\0');
}

void LightGUI::_SetMesh(DWORD_PTR _dw1, DWORD_PTR _dw)
{
	string strKey = (const char*)_dw1;
	tstring tstrKey;
	StringToTString(strKey, tstrKey);
	SharedPtr<CMesh> pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(tstrKey);
	assert(pMesh.Get());
	GetTargetObject()->MeshRenderer()->SetMesh(pMesh);
}

void LightGUI::_SetMatrial(DWORD_PTR _dw1, DWORD_PTR _dw)
{
	string strKey = (const char*)_dw1;
	tstring tstrKey;
	StringToTString(strKey, tstrKey);
	SharedPtr<CMaterial> pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(tstrKey);
	assert(pMtrl.Get());
	GetTargetObject()->MeshRenderer()->SetMaterial(pMtrl, 0);
}

LightGUI::LightGUI(E_ComponentType _eType) :
	ComponentGUI(_eType)
{
	_InitComboBoxList();
	string strComponent;
	TStringToString(ComponentTypeToStr(_eType), strComponent);
}

LightGUI::~LightGUI()
{
}