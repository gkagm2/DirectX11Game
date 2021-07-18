#include "pch.h"
#include "CScript.h"

CScript::CScript(UINT _iType) :
	CComponent(E_ComponentType::Script),
	m_iScriptType(_iType)
{
}

CScript::~CScript()
{
}