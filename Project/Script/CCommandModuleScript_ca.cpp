#include "pch.h"
#include "CCommandModuleScript_ca.h"

CCommandModuleScript_ca::CCommandModuleScript_ca() :
	CModuleScript_ca((UINT)SCRIPT_TYPE::COMMANDMODULESCRIPT_CA)
{
	SetWeight(15000.f);
	SetHp(1600.f);
}

CCommandModuleScript_ca::~CCommandModuleScript_ca()
{
}

void CCommandModuleScript_ca::Start()
{
}

void CCommandModuleScript_ca::Update()
{
}

void CCommandModuleScript_ca::InitModule(E_ModuleLevel_ca _eLevel)
{
	switch (_eLevel) {
	case E_ModuleLevel_ca::Alpha:
		SetWeight(20000.f);
		SetHp(500.f);
		break;
	case E_ModuleLevel_ca::Bravo:
		SetWeight(21000.f);
		SetHp(1000.f);
		break;
	case E_ModuleLevel_ca::Charlie:
		SetWeight(22000.f);
		SetHp(2000.f);
		break;
	case E_ModuleLevel_ca::Delta:
		SetWeight(23000.f);
		SetHp(4000.f);
		break;
	case E_ModuleLevel_ca::Echo:
		SetWeight(24000.f);
		SetHp(8000.f);
		break;
	case E_ModuleLevel_ca::Foxtrot:
		SetWeight(25000.f);
		SetHp(16000.f);
		break;
	case E_ModuleLevel_ca::Golf:
		SetWeight(26000.f);
		SetHp(32000.f);
		break;
	case E_ModuleLevel_ca::Hotel:
		SetWeight(27000.f);
		SetHp(64000.f);
		break;
	case E_ModuleLevel_ca::India:
		SetWeight(28000.f);
		SetHp(128000.f);
		break;
	case E_ModuleLevel_ca::Juliet:
		SetWeight(29000.f);
		SetHp(256000.f);
		break;
	case E_ModuleLevel_ca::Kilo:
		SetWeight(30000.f);
		SetHp(512000.f);
		break;
	default:
		assert(nullptr);
		break;
	}
}