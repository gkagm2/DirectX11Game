#include "pch.h"
#include "TileMapGUI.h"
#include <Engine\CResourceManager.h>
#include <Engine\CTileMap.h>
#include "ListViewGUI.h"
#include "CImGuiManager.h"
#include "MeshRendererGUI.h"
#include "ParamGUI.h"
#include "TileMapEditorGUI.h"

TileMapGUI::TileMapGUI() :
	ComponentGUI(E_ComponentType::TileMap)
{
}

TileMapGUI::~TileMapGUI()
{
}

void TileMapGUI::Update()
{
	if (false == Start())
		return;

	CTileMap* pTileMap = GetTargetObject()->TileMap();

	// �ؽ��� ����
	static ImVec2 m_vSize;			// Size of the image
	static ImVec2 m_vUvMin;		// Left-Top
	static ImVec2 m_vUvMax;		// Right-Bottom
	static ImVec4 m_v4BGColor;		// Background color
	static ImVec4 m_vtintColor;
	static ImVec4 m_v4BorderColor;

	CTexture* pTex = pTileMap->GetAtlasTexture().Get();
	if (pTex) {
		string strKey;
		TStringToString(pTex->GetKey().c_str(), strKey);

		if (ParamGUI::Render_Texture(strKey.c_str(), pTex, this, (GUI_CALLBACK)&TileMapGUI::SelectTexture)) {
		}
	}
	

	ShowMeshRenderer();

	if (ImGui::Button("Editor Open")) {
		TileMapEditorGUI* pTileMapEditorGUI = dynamic_cast<TileMapEditorGUI*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_TileMapEditor));
		if (!pTileMapEditorGUI)
			assert(nullptr && _T("Ÿ�ϸ� �����͸� �� �� ����."));
		else {
			pTileMapEditorGUI->SetActive(true);
			pTileMapEditorGUI->SetTargetObject(GetTargetObject());
		}
	}

	End();
}

void TileMapGUI::ShowMeshRenderer()
{
	CTileMap* pTileMap = GetTargetObject()->TileMap();
	
	CMeshRenderer* pMeshRenderer = GetTargetObject()->MeshRenderer();
	SharedPtr<CMesh> pMesh = pTileMap->GetMesh();
	SharedPtr<CMaterial> pMaterial = pTileMap->GetMaterial();

	char strMeshName[255];
	GetResourceName(pMesh.Get(), strMeshName, 255);

	char strMaterialName[255];
	GetResourceName(pMaterial.Get(), strMaterialName, 255);
	ImGui::Text("Mesh    ");
	ImGui::SameLine();
	ImGui::InputText("##Mesh", strMeshName, 255, ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	if (ImGui::Button("Select##Mesh")) {
		// ����Ʈ �並 �����ش�.
		ListViewGUI* pListViewGUI = dynamic_cast<ListViewGUI*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_ListView));
		assert(pListViewGUI);


		vector<tstring> vecNames;
		CResourceManager::GetInstance()->GetResourceKeys(E_ResourceType::Mesh, vecNames);
		pListViewGUI->SetList(vecNames, ResourceTypeToStr(E_ResourceType::Mesh));
		pListViewGUI->SetDoubleClickCallBack(this, (GUI_CALLBACK)&TileMapGUI::_SetMesh);
		pListViewGUI->SetActive(true);
	}


	ImGui::Text("Material");
	ImGui::SameLine();
	ImGui::InputText("##Material", strMaterialName, 255, ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	if (ImGui::Button("Select##Material")) {
		// ����Ʈ �並 �����ش�.
		ListViewGUI* pListViewGUI = dynamic_cast<ListViewGUI*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_ListView));
		assert(pListViewGUI);
		if (pListViewGUI) {
			vector<tstring> vecNames;
			CResourceManager::GetInstance()->GetResourceKeys(E_ResourceType::Material, vecNames);
			pListViewGUI->SetList(vecNames, ResourceTypeToStr(E_ResourceType::Material));
			pListViewGUI->SetDoubleClickCallBack(this, (GUI_CALLBACK)&TileMapGUI::_SetMatrial);
			pListViewGUI->SetActive(true);
		}
	}
}


void TileMapGUI::SelectTexture(DWORD_PTR _pStr, DWORD_PTR _NONE)
{
	// ������ �ؽ��ĸ� �˾Ƴ���.
	const char* pStr = (const char*)_pStr;
	string str = pStr;
	tstring tStrKey;
	StringToTString(str, tStrKey);

	CTexture* pTex = CResourceManager::GetInstance()->FindRes<CTexture>(tStrKey).Get();
	assert(pTex);
	GetTargetObject()->TileMap()->SetTileAtlas(pTex);
}

void TileMapGUI::_SetMesh(DWORD_PTR _dw1, DWORD_PTR _dw)
{
	
	string strKey = (const char*)_dw1;
	tstring tstrKey;
	StringToTString(strKey, tstrKey);
	SharedPtr<CMesh> pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(tstrKey);
	assert(pMesh.Get());
	CTileMap* pTileMap = GetTargetObject()->TileMap();
	pTileMap->SetMesh(pMesh);
}

void TileMapGUI::_SetMatrial(DWORD_PTR _dw1, DWORD_PTR _dw)
{
	string strKey = (const char*)_dw1;
	tstring tstrKey;
	StringToTString(strKey, tstrKey);
	SharedPtr<CMaterial> pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(tstrKey);
	assert(pMtrl.Get());
	CTileMap* pTileMap = GetTargetObject()->TileMap();
	pTileMap->SetMaterial(pMtrl);
}