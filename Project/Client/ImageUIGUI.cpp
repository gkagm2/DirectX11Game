#include "pch.h"
#include "ImageUIGUI.h"

ImageUIGUI::ImageUIGUI() :
	ComponentGUI(E_ComponentType::ImageUI)
{
}

ImageUIGUI::~ImageUIGUI()
{
}

void ImageUIGUI::Update()
{
	if (false == Start())
		return;

	End();
}