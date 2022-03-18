#include "pch.h"
#include "LandscapeGUI.h"
#include "ParamGUI.h"

LandscapeGUI::LandscapeGUI() :
	ComponentGUI(E_ComponentType::Landscape)
{
}

LandscapeGUI::~LandscapeGUI()
{
}

void LandscapeGUI::Update()
{
	if (Start()) {
		
		// quad
		// 7
		// 15
		// 31
		// 63
		// 127
		// 255

		//static int iCurQuad = 0;
		//ParamGUI::Render_ComboBox("Quad", iCurQuad, )



		// component 
		// 1 by 1
		// 2 by 2

		
	}

	End();
}