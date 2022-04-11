#include "pch.h"
#include "Collider3DGUI.h"

Collider3DGUI::Collider3DGUI() : 
	ComponentGUI(E_ComponentType::Collider3D)
{
}

Collider3DGUI::~Collider3DGUI()
{
}

void Collider3DGUI::Update()
{
	if (false == Start())
		return;

	End();
}