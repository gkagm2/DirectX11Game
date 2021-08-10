#include "pch.h"
#include "ContextMenuGUI.h"

ContextMenuGUI::ContextMenuGUI() :
	m_bPopUp(false),
	m_pClickCallBack(nullptr),
	m_pInst(nullptr),
	m_pGClickCallBack(nullptr)
{
}

ContextMenuGUI::~ContextMenuGUI()
{
}

void ContextMenuGUI::Update()
{
}

void ContextMenuGUI::SetList(const vector<tstring>& _vecNames)
{
	string strName;

	// 리스트 문자열 세팅
	m_vecListName.clear();
	for (UINT i = 0; i < _vecNames.size(); ++i) {
		TStringToString(_vecNames[i], strName);
		m_vecListName.push_back(strName);
	}

	// 보여줄 문자열 세팅
	m_vecListAdr.clear();
	for (const auto& name : m_vecListName)
		m_vecListAdr.push_back(name.c_str());
}

inline void ContextMenuGUI::SetActive(bool _bIsActive)
{
	GUI::SetActive(_bIsActive);
	m_bGUIOpen = true;
}