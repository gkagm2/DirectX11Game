#include "pch.h"
#include "CMainPanel_bu.h"

CMainPanel_bu::CMainPanel_bu() :
	CScript((UINT)SCRIPT_TYPE::MAINPANEL_BU)
	/*m_pStartBtn(nullptr),
	m_pOptionBtn(nullptr)*/
{
	TScriptParam param;
	param.eParam = E_ScriptParam::GAMEOBJ;
	param.pData = GetGameObject();
	param.strName = _T("StartBtn obj");
	AddParam(param);

	param.eParam = E_ScriptParam::GAMEOBJ;
	param.pData = GetGameObject();
	param.strName = _T("OptionBtn obj");
	AddParam(param);
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
}

void CMainPanel_bu::Start()
{
}

void CMainPanel_bu::Update()
{
}