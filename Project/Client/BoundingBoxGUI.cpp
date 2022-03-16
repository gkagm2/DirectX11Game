#include "pch.h"
#include "BoundingBoxGUI.h"

BoundingBoxGUI::BoundingBoxGUI() :
	ComponentGUI(E_ComponentType::BoundingBox)
{
}

BoundingBoxGUI::~BoundingBoxGUI()
{
}

void BoundingBoxGUI::Update()
{
	if (false == Start())
		return;

	End();
}