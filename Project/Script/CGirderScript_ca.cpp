#include "pch.h"
#include "CGirderScript_ca.h"

CGirderScript_ca::CGirderScript_ca() :
	CModuleScript_ca((UINT)SCRIPT_TYPE::GIRDERSCRIPT_CA)
{
}

CGirderScript_ca::~CGirderScript_ca()
{
}

void CGirderScript_ca::Start()
{
}

void CGirderScript_ca::Update()
{
}

void CGirderScript_ca::OnCollisionEnter2D(CCollider2D* _pOther)
{
}

void CGirderScript_ca::OnCollisionStay2D(CCollider2D* _pOther)
{
}

void CGirderScript_ca::OnCollisionExit2D(CCollider2D* _pOther)
{
}

void CGirderScript_ca::InitModule(E_ModuleLevel_ca _eLevel)
{
	switch (_eLevel) {
	case E_ModuleLevel_ca::Alpha:
		SetWeight(1000.f);
		SetHp(2000.f);
		break;
	case E_ModuleLevel_ca::Bravo:
		SetWeight(2000.f);
		SetHp(4000.f);
		break;
	case E_ModuleLevel_ca::Charlie:
		SetWeight(3000.f);
		SetHp(8000.f);
		break;
	case E_ModuleLevel_ca::Delta:
		SetWeight(8000.f);
		SetHp(16000.f);
		break;
	case E_ModuleLevel_ca::Echo:
		SetWeight(10000.f);
		SetHp(32000.f);
		break;
	case E_ModuleLevel_ca::Foxtrot:
		SetWeight(12000.f);
		SetHp(64000.f);
		break;
	case E_ModuleLevel_ca::Golf:
		SetWeight(21000.f);
		SetHp(128000.f);
		break;
	case E_ModuleLevel_ca::Hotel:
		SetWeight(24000.f);
		SetHp(256000.f);
		break;
	case E_ModuleLevel_ca::India:
		SetWeight(27000.f);
		SetHp(512000.f);
		break;
	case E_ModuleLevel_ca::Juliet:
		SetWeight(40000.f);
		SetHp(1024000.f);
		break;
	case E_ModuleLevel_ca::Kilo:
		SetWeight(20000.f);
		SetHp(2048000.f);
		break;
	default:
		assert(nullptr);
		break;
	}
}
