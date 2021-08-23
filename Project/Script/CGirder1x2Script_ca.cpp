#include "pch.h"
#include "CGirder1x2Script_ca.h"

CGirder1x2Script_ca::CGirder1x2Script_ca() :
	CGirderScript_ca((UINT)SCRIPT_TYPE::GIRDER1X2SCRIPT_CA)
{
}

CGirder1x2Script_ca::~CGirder1x2Script_ca()
{
}

void CGirder1x2Script_ca::Start()
{
	InitModule(E_ModuleLevel_ca::Alpha);
	InitModuleSize(E_ModuleSize_ca::Size1x2);
}

void CGirder1x2Script_ca::Update()
{
}

void CGirder1x2Script_ca::OnCollisionEnter2D(CCollider2D* _pOther)
{
}

void CGirder1x2Script_ca::OnCollisionStay2D(CCollider2D* _pOther)
{
}

void CGirder1x2Script_ca::OnCollisionExit2D(CCollider2D* _pOther)
{
}

