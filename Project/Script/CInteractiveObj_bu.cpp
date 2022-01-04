#include "pch.h"
#include "CInteractiveObj_bu.h"

CInteractiveObj_bu::CInteractiveObj_bu() :
	CScript((UINT)SCRIPT_TYPE::INTERACTIVEOBJ_BU),
	m_ActivateCallBack(nullptr),
	m_DeActivateCallBack(nullptr)
{
	assert(nullptr && _T(" 상속 받아서 사용해야 됨"));
}

CInteractiveObj_bu::CInteractiveObj_bu(UINT _iScriptType) :
	CScript(_iScriptType),
	m_ActivateCallBack(nullptr),
	m_DeActivateCallBack(nullptr)
{
}

CInteractiveObj_bu::~CInteractiveObj_bu()
{
}
