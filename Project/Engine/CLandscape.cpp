#include "pch.h"
#include "CLandscape.h"

CLandscape::CLandscape() :
	CComponent(E_ComponentType::Landscape)
{
}

CLandscape::~CLandscape()
{
}


void CLandscape::UpdateData()
{
}

void CLandscape::FinalUpdate()
{
}

bool CLandscape::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);
	return true;
}

bool CLandscape::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);
	return true;
}
