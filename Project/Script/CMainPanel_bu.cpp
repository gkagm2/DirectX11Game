#include "pch.h"
#include "CMainPanel_bu.h"
#include <Engine\CUI.h>
#include <Script\CUIManager_bu.h>
#include <Script\CUIContainer_bu.h>
#include <Script\CInGamePanel_bu.h>

CMainPanel_bu::CMainPanel_bu() :
	CScript((UINT)SCRIPT_TYPE::MAINPANEL_BU),
	m_pStartBtnObj(nullptr),
	m_pOptionBtnObj(nullptr),
	m_pStartBtn(nullptr),
	m_pOptionBtn(nullptr)
{
	AddParam(TScriptParam{ _T("Start Btn Obj"), E_ScriptParam::GAMEOBJ, (void**)&m_pStartBtnObj });
	AddParam(TScriptParam{ _T("Option Btn Obj"), E_ScriptParam::GAMEOBJ, (void**)&m_pOptionBtnObj });
}

CMainPanel_bu::~CMainPanel_bu()
{
}


void CMainPanel_bu::Awake()
{
	m_pStartBtn = m_pStartBtnObj->GetComponent<CButtonUI>();
	assert(m_pStartBtn);
	m_pOptionBtn = m_pOptionBtnObj->GetComponent<CButtonUI>();
	assert(m_pOptionBtn);

	m_pStartBtn->SetClickCallBack(this, &CMainPanel_bu::OnStartBtnClick);
	m_pOptionBtn->SetClickCallBack(this, &CMainPanel_bu::OnOptionBtnClick);
}

void CMainPanel_bu::Start()
{
}

void CMainPanel_bu::Update()
{

}

bool CMainPanel_bu::SaveToScene(FILE* _pFile)
{
	FWriteLinkObj(m_pStartBtnObj, _pFile);
	FWriteLinkObj(m_pOptionBtnObj, _pFile);
	return true;
}

bool CMainPanel_bu::LoadFromScene(FILE* _pFile)
{
	FReadLinkObj((CObject**)&m_pStartBtnObj, _pFile);
	FReadLinkObj((CObject**)&m_pOptionBtnObj, _pFile);

	return true;
}

void CMainPanel_bu::OnStartBtnClick()
{
	CUIManager_bu* pUIMgr = nullptr;
	FIND_Component(pUIMgr, CUIManager_bu);
	if (pUIMgr) {
		CInGamePanel_bu* pInGamePanel = pUIMgr->GetContainer()->GetInGamePanel();
		pInGamePanel->GetGameObject()->SetActive(true, true);
		GetGameObject()->SetActive(false, true);
	}
}

void CMainPanel_bu::OnOptionBtnClick()
{
}