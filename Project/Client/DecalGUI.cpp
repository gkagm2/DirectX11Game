#include "pch.h"
#include "DecalGUI.h"
#include "CImGuiManager.h"
#include "ParamGUI.h"

DecalGUI::DecalGUI() :
	ComponentGUI(E_ComponentType::Decal)
{
}

DecalGUI::~DecalGUI()
{
}

void DecalGUI::Update()
{
	if (false == Start())
		return;	

	ImGui::Text("Material");

	End();
}