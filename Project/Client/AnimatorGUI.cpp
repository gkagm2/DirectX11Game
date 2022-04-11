#include "pch.h"
#include "AnimatorGUI.h"

AnimatorGUI::AnimatorGUI(E_ComponentType _eType) :
	ComponentGUI(_eType)
{
	string strName;
	TStringToString(ComponentTypeToStr(_eType), strName);
	SetName(strName);
}

AnimatorGUI::~AnimatorGUI()
{
}

void AnimatorGUI::Update()
{
	if (false == Start())
		return;

	End();
}
