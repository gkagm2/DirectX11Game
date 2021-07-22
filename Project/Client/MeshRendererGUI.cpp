#include "pch.h"
#include "MeshRendererGUI.h"
#include <Engine\CMeshRenderer.h>

MeshRendererGUI::MeshRendererGUI() :
	ComponentGUI(E_ComponentType::MeshRenderer)
{
}

MeshRendererGUI::~MeshRendererGUI()
{
}

void MeshRendererGUI::Update()
{
	if (false == Start())
		return;

	CMeshRenderer* pMeshRenderer = GetTargetObject()->MeshRenderer();
	SharedPtr<CMesh> pMesh = pMeshRenderer->GetMesh();
	SharedPtr<CMaterial> pMaterial = pMeshRenderer->GetSharedMaterial();

	char strMeshName[255];
	GetResourceName(pMesh.Get(), strMeshName, 255);

	char strMaterialName[255];
	GetResourceName(pMaterial.Get(), strMaterialName, 255);
	
	ImGui::Text("Mesh    "); ImGui::SameLine(); ImGui::InputText("##Mesh", strMeshName, 255, ImGuiInputTextFlags_ReadOnly);
	ImGui::Text("Material"); ImGui::SameLine(); ImGui::InputText("##Material", strMaterialName, 255, ImGuiInputTextFlags_ReadOnly);

	End();
}