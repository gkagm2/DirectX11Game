#include "pch.h"
#include "CModuleScript_ca.h"

CModuleScript_ca::CModuleScript_ca() :
	CScript((UINT)SCRIPT_TYPE::MODULESCRIPT_CA),
	m_pParentModuleGameObj(nullptr),
	m_eModuleLevel(E_ModuleLevel_ca::Alpha),
	m_eModuleType(E_ModuleType_ca::Girder),
	m_fHp(1.f),
	m_fWeight(1.f)
{
}

CModuleScript_ca::CModuleScript_ca(UINT _iScriptNum) :
	CScript(_iScriptNum),
	m_pParentModuleGameObj(nullptr),
	m_eModuleLevel(E_ModuleLevel_ca::Alpha),
	m_eModuleType(E_ModuleType_ca::Girder),
	m_fHp(1.f),
	m_fWeight(1.f)
{
}

CModuleScript_ca::~CModuleScript_ca()
{
}

void CModuleScript_ca::Start()
{
}

void CModuleScript_ca::Update()
{
}