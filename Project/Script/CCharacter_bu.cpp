#include "pch.h"
#include "CCharacter_bu.h"

CCharacter_bu::CCharacter_bu() :
	CScript((UINT)SCRIPT_TYPE::CHARACTER_BU)
{
}

CCharacter_bu::CCharacter_bu(UINT _iScriptType) :
	CScript(_iScriptType)
{
}

CCharacter_bu::~CCharacter_bu()
{
}
