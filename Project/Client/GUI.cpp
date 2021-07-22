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
	assert(_pResource);
#ifdef UNICODE
	tstring strKey = _pResource->GetKey();
	WStringToString(strKey, _pOut);
#elif
	_pOut = _pResource->GetKey();
#endif
}

void GUI::GetResourceName(CResource* _pResource, char* _pOut, int _size)
{
	assert(_pResource);
	string strKey;
#ifdef UNICODE
	tstring strTemp = _pResource->GetKey();
	WStringToString(strTemp, strKey);
#elif
	strKey = _pResource->GetKey();
#endif
	strcpy_s(_pOut, _size, strKey.c_str());
}