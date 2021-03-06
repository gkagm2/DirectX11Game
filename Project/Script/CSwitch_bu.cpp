#include "pch.h"
#include "CSwitch_bu.h"
#include <Engine\CAnimator2D.h>
#include <Engine\CScript.h>

CSwitch_bu::CSwitch_bu() :
	CInteractiveObj_bu((UINT)SCRIPT_TYPE::SWITCH_BU),
	m_pAnim(nullptr),
	m_bSwitchState(true),
	m_pTargetObj(nullptr),
	m_iOffIdx(0),
	m_iOnIdx(0)
{
	AddParam(TScriptParam{ _T("Cur Switch State"), E_ScriptParam::BOOL, &m_bSwitchState });
	AddParam(TScriptParam{ _T("On Anim Idx"), E_ScriptParam::INT, &m_iOnIdx });
	AddParam(TScriptParam{ _T("Off Anim Idx"), E_ScriptParam::INT,&m_iOffIdx });
}

CSwitch_bu::CSwitch_bu(const CSwitch_bu& _origin) :
	CInteractiveObj_bu((UINT)SCRIPT_TYPE::SWITCH_BU),
	m_pAnim(nullptr),
	m_bSwitchState(_origin.m_bSwitchState),
	m_pTargetObj(nullptr),
	m_iOffIdx(_origin.m_iOffIdx),
	m_iOnIdx(_origin.m_iOnIdx)
{
	AddParam(TScriptParam{ _T("Cur Switch State"), E_ScriptParam::BOOL, &m_bSwitchState });
	AddParam(TScriptParam{ _T("On Anim Idx"), E_ScriptParam::INT, &m_iOnIdx });
	AddParam(TScriptParam{ _T("Off Anim Idx"), E_ScriptParam::INT,&m_iOffIdx });
}

CSwitch_bu::~CSwitch_bu()
{
}

void CSwitch_bu::Awake()
{
	assert(Animator2D());
	m_pAnim = Animator2D();
	int iCurFrame = m_pAnim->GetCurAnimation()->GetCurFrameIdx();


	m_pTargetObj = GetGameObject()->FindGameObjectSameLine(_T("InteractiveObj"));
	assert(m_pTargetObj);
	m_pLight = GetGameObject()->FindComponentInChilds<CLight2D>();
	assert(m_pLight);
}

void CSwitch_bu::Start()
{
}

void CSwitch_bu::Update()
{
	Interaction(m_bSwitchState);
}

void CSwitch_bu::Interaction(bool _bActive)
{
	if (_bActive) {
		if (m_ActivateCallBack)
			m_ActivateCallBack();
		m_pAnim->SetCurAnimationFrame(m_iOnIdx);
		if (!m_pLight->GetGameObject()->IsActive())
			m_pLight->GetGameObject()->SetActive(true);
	}
	else {
		if (m_DeActivateCallBack)
			m_DeActivateCallBack();
		m_pAnim->SetCurAnimationFrame(m_iOffIdx);
		if(m_pLight->GetGameObject()->IsActive())
			m_pLight->GetGameObject()->SetActive(false);
	}
	if (m_pTargetObj) {
		CInteractiveObj_bu* pInter = m_pTargetObj->GetComponent<CInteractiveObj_bu>();
		pInter->Interaction(_bActive);
	}

	m_bIsActivate = _bActive;
	m_bSwitchState = _bActive;
}

bool CSwitch_bu::SaveToScene(FILE* _pFile)
{
	FWrite(m_bSwitchState, _pFile);
	FWrite(m_iOffIdx, _pFile);
	FWrite(m_iOnIdx, _pFile);
	return true;
}

bool CSwitch_bu::LoadFromScene(FILE* _pFile)
{
	FRead(m_bSwitchState, _pFile);
	m_bIsActivate = m_bSwitchState;
	FRead(m_iOffIdx, _pFile);
	FRead(m_iOnIdx, _pFile);
	return true;
}
