#include "pch.h"
#include "CSwitch_bu.h"
#include <Engine\CAnimator2D.h>

void CSwitch_bu::SwitchActive(bool _bActive)
{
	if (_bActive) {
		if (m_ActivateCallBack)
			m_ActivateCallBack();
		m_pAnim->SetCurAnimationFrame(m_iOnIdx);
	}
	else {
		if (m_DeActivateCallBack)
			m_DeActivateCallBack();
		m_pAnim->SetCurAnimationFrame(m_iOffIdx);
	}
	m_bSwitchState = _bActive;
}

CSwitch_bu::CSwitch_bu() :
	CScript((UINT)SCRIPT_TYPE::SWITCH_BU),
	m_pAnim(nullptr),
	m_bSwitchState(true),
	m_ActivateCallBack(nullptr),
	m_DeActivateCallBack(nullptr)
{
	AddParam(TScriptParam{ _T("Cur Switch State"), E_ScriptParam::BOOL, &m_bSwitchState});
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
}

void CSwitch_bu::Start()
{
}

void CSwitch_bu::Update()
{
	SwitchActive(m_bSwitchState);
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
	FRead(m_iOffIdx, _pFile);
	FRead(m_iOnIdx, _pFile);
	return true;
}
