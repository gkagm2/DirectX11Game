#include "pch.h"
#include "Animator3DGUI.h"

Animator3DGUI::Animator3DGUI() :
	AnimatorGUI(E_ComponentType::Animator3D)
{
	_InitComboBox();
}

Animator3DGUI::~Animator3DGUI()
{
}

void Animator3DGUI::Update()
{
	if (false == Start())
		return;

	End();
}

void Animator3DGUI::_InitComboBox()
{
}
