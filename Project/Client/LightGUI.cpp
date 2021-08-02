#include "pch.h"
#include "LightGUI.h"

LightGUI::LightGUI(E_ComponentType _eType) :
	ComponentGUI(_eType)
{
	string strComponent;
	TStringToString(ComponentTypeToStr(_eType), strComponent);
}

LightGUI::~LightGUI()
{
}