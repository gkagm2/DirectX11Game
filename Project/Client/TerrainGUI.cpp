#include "pch.h"
#include "TerrainGUI.h"
#include "ParamGUI.h"
#include <Engine\CTerrain.h>

TerrainGUI::TerrainGUI() :
	ComponentGUI(E_ComponentType::Terrain)
{
	Init();
}

TerrainGUI::~TerrainGUI()
{
}

void TerrainGUI::Init()
{
	m_vecQuadList.push_back("7x7 Quad");
	m_vecQuadList.push_back("15x15 Quad");
	m_vecQuadList.push_back("31x31 Quad");
	m_vecQuadList.push_back("63x63 Quad");
	m_vecQuadList.push_back("127x127 Quad");
	m_vecQuadList.push_back("255x255 Quad");

	m_vecQuadNumList.push_back(7);
	m_vecQuadNumList.push_back(15);
	m_vecQuadNumList.push_back(31);
	m_vecQuadNumList.push_back(63);
	m_vecQuadNumList.push_back(127);
	m_vecQuadNumList.push_back(255);

	m_vecComponentList.push_back("1x1 Comp");
	m_vecComponentList.push_back("2x2 Comp");

	m_vecCompNumList.push_back(1);
	m_vecCompNumList.push_back(2);
}

void TerrainGUI::Update()
{
	if (Start()) {
		
		UINT iQuadX = GetTargetObject()->Terrain()->GetQuadX();
		UINT iQuadZ = GetTargetObject()->Terrain()->GetQuadZ();
		UINT iCompX = GetTargetObject()->Terrain()->GetComponentX();
		UINT iCompZ = GetTargetObject()->Terrain()->GetComponentZ();

		// QUAD
		ImGui::Text("Current Quad %dx%d", iQuadX, iQuadZ);

		static int iCurQuad = 0;
		ParamGUI::Render_ComboBox("Quad", &iCurQuad, m_vecQuadList);


		// COMPONENT 
		ImGui::Text("Cur Compnoent %dx%d", iCompX, iCompZ);

		static int iCurComponent = 0;
		ParamGUI::Render_ComboBox("Component", &iCurComponent, m_vecComponentList);


		if (ImGui::Button("Create##Terrain")) {
			iQuadX = iQuadZ = m_vecQuadNumList[iCurQuad];
			iCompX = iCompZ = m_vecCompNumList[iCurComponent];
			GetTargetObject()->Terrain()->SetQuard(iQuadX, iQuadZ);
			GetTargetObject()->Terrain()->SetComponent(iCompX, iCompZ);
			GetTargetObject()->Terrain()->Create();
		}
	}

	End();
}