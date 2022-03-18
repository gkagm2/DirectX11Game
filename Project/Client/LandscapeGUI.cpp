#include "pch.h"
#include "LandscapeGUI.h"
#include "ParamGUI.h"
#include <Engine\CLandscape.h>

LandscapeGUI::LandscapeGUI() :
	ComponentGUI(E_ComponentType::Landscape)
{
	Init();
}

LandscapeGUI::~LandscapeGUI()
{
}

void LandscapeGUI::Init()
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

void LandscapeGUI::Update()
{
	if (Start()) {
		
		UINT iQuadX = GetTargetObject()->Landscape()->GetQuadX();
		UINT iQuadZ = GetTargetObject()->Landscape()->GetQuadZ();
		UINT iCompX = GetTargetObject()->Landscape()->GetComponentX();
		UINT iCompZ = GetTargetObject()->Landscape()->GetComponentZ();

		// QUAD
		ImGui::Text("Current Quad %dx%d", iQuadX, iQuadZ);

		static int iCurQuad = 0;
		ParamGUI::Render_ComboBox("Quad", &iCurQuad, m_vecQuadList);


		// COMPONENT 
		ImGui::Text("Cur Compnoent %dx%d", iCompX, iCompZ);

		static int iCurComponent = 0;
		ParamGUI::Render_ComboBox("Component", &iCurComponent, m_vecComponentList);


		if (ImGui::Button("Create##Landscape")) {
			iQuadX = iQuadZ = m_vecQuadNumList[iCurQuad];
			iCompX = iCompZ = m_vecCompNumList[iCurComponent];
			GetTargetObject()->Landscape()->SetQuard(iQuadX, iQuadZ);
			GetTargetObject()->Landscape()->SetComponent(iCompX, iCompZ);
			GetTargetObject()->Landscape()->Create();
		}
	}

	End();
}