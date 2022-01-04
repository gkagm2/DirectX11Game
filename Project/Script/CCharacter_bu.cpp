#include "pch.h"
#include "CCharacter_bu.h"

CCharacter_bu::CCharacter_bu() :
	CScript((UINT)SCRIPT_TYPE::CHARACTER_BU),
	m_fJumpPower{ 10.f },
	m_fMovePower{ 30.f }
{
}

CCharacter_bu::CCharacter_bu(UINT _iScriptType) :
	CScript(_iScriptType),
	m_fJumpPower{ 10.f },
	m_fMovePower{ 3.f }
{
}

CCharacter_bu::~CCharacter_bu()
{
}