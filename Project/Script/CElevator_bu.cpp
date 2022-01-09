#include "pch.h"
#include "CElevator_bu.h"

CElevator_bu::CElevator_bu() :
	CInteractiveObj_bu((UINT)SCRIPT_TYPE::ELEVATOR_BU),
	m_fMoveTime(0.f),
	m_bMove(false),
	m_fSpeed(3.f),
	m_vStartPos{},
	m_vEndPos{},
	m_eType{ E_ElevationType::Once }
{
	AddParam(TScriptParam(_T("Move Speed"), E_ScriptParam::FLOAT, &m_fSpeed));
	AddParam(TScriptParam(_T("Type 0 : once, 1 : loop"), E_ScriptParam::BOOL, &m_eType));
}

CElevator_bu::CElevator_bu(const CElevator_bu& _origin) :
	CInteractiveObj_bu((UINT)SCRIPT_TYPE::ELEVATOR_BU),
	m_fMoveTime(_origin.m_fMoveTime),
	m_bMove(_origin.m_bMove),
	m_fSpeed(_origin.m_fSpeed),
	m_vStartPos{_origin.m_vStartPos},
	m_vEndPos{_origin.m_vEndPos},
	m_eType{_origin.m_eType}
{
	AddParam(TScriptParam(_T("Move Speed"), E_ScriptParam::FLOAT, &m_fSpeed));
	AddParam(TScriptParam(_T("Type 0 : once, 1 : loop"), E_ScriptParam::BOOL, &m_eType));
}

CElevator_bu::~CElevator_bu()
{
}

void CElevator_bu::Start()
{
	SetActivateCallBack(this, &CElevator_bu::Move);
	SetDeActivateCallBack(this, &CElevator_bu::Stop);
}

void CElevator_bu::Update()
{
}

void CElevator_bu::Interaction(bool _bActive)
{

}

void CElevator_bu::Move()
{
	m_bIsActivate = true;
	m_bMove = true;
}

void CElevator_bu::Stop()
{
	m_bIsActivate = false;
	m_bMove = false;
}
