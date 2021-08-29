#include "pch.h"
#include "ButtonUIGUI.h"

ButtonUIGUI::ButtonUIGUI() :
	ComponentGUI(E_ComponentType::ButtonUI)
{
}

ButtonUIGUI::~ButtonUIGUI()
{
}

void ButtonUIGUI::Update()
{
	if (false == Start())
		return;


	End();
}