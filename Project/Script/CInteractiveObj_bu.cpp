#include "pch.h"
#include "CInteractiveObj_bu.h"

CInteractiveObj_bu::CInteractiveObj_bu() :
	CScript((UINT)SCRIPT_TYPE::INTERACTIVEOBJ_BU),
	m_ActivateCallBack(nullptr),
	m_DeActivateCallBack(nullptr),
	m_bIsActivate(false)
{
	assert(nullptr && _T(" ��� �޾Ƽ� ����ؾ� ��"));
}

CInteractiveObj_bu::CInteractiveObj_bu(UINT _iScriptType) :
	CScript(_iScriptType),
	m_ActivateCallBack(nullptr),
	m_DeActivateCallBack(nullptr),
	m_bIsActivate(false)
{
}

CInteractiveObj_bu::~CInteractiveObj_bu()
{
}
