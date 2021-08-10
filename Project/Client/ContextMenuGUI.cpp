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

	// ����Ʈ ���ڿ� ����
	m_vecListName.clear();
	for (UINT i = 0; i < _vecNames.size(); ++i) {
		TStringToString(_vecNames[i], strName);
		m_vecListName.push_back(strName);
	}

	// ������ ���ڿ� ����
	m_vecListAdr.clear();
	for (const auto& name : m_vecListName)
		m_vecListAdr.push_back(name.c_str());
}

inline void ContextMenuGUI::SetActive(bool _bIsActive)
{
	GUI::SetActive(_bIsActive);
	m_bGUIOpen = true;
}