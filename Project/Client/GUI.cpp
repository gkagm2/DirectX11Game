#include "pch.h"
#include "GUI.h"
#include <Engine\CResource.h>

GUI::GUI() :
	m_bGUIOpen(true),
	m_bActive(true)
{
}

GUI::~GUI()
{
}

void GUI::GetResourceName(CResource* _pResource, string& _pOut)
{
	//assert(_pResource);
	if (nullptr != _pResource)
		TStringToString(_pResource->GetKey(), _pOut);
}

void GUI::GetResourceName(CResource* _pResource, char* _pOut, int _size)
{
	//assert(_pResource);
	if (nullptr != _pResource) {
		string strKey;
		TStringToString(_pResource->GetKey(), strKey);
		strcpy_s(_pOut, _size, strKey.c_str());
	}
}