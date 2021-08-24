#include "pch.h"
#include "CBoosterScript_ca.h"

CBoosterScript_ca::CBoosterScript_ca() :
	CModuleScript_ca((UINT)SCRIPT_TYPE::BOOSTERSCRIPT_CA, E_ModuleType_ca::Booster, E_ModuleLevel_ca::Alpha, E_ModuleSize_ca::Size1x1),
	m_fForce(1.f)
{
}

CBoosterScript_ca::~CBoosterScript_ca()
{
}

void CBoosterScript_ca::Start()
{
}

void CBoosterScript_ca::Update()
{
}

void CBoosterScript_ca::_InitModuleLevel(E_ModuleLevel_ca _eLevel)
{
	switch (_eLevel) {
	case E_ModuleLevel_ca::Alpha:
		SetWeight(1000.f);
		SetHp(1000.f);
		SetForce(120000.f);
		break;
	case E_ModuleLevel_ca::Bravo:
		SetWeight(2000.f);
		SetHp(2000.f);
		SetForce(240000.f);
		break;
	case E_ModuleLevel_ca::Charlie:
		SetWeight(3000.f);
		SetHp(4000.f);
		SetForce(360000.f);
		break;
	case E_ModuleLevel_ca::Delta:
		SetWeight(4000.f);
		SetHp(8000.f);
		SetForce(480000.f);
		break;
	case E_ModuleLevel_ca::Echo:
		SetWeight(5000.f);
		SetHp(16000.f);
		SetForce(600000.f);
		break;
	case E_ModuleLevel_ca::Foxtrot:
		SetWeight(6000.f);
		SetHp(32000.f);
		SetForce(720000.f);
		break;
	case E_ModuleLevel_ca::Golf:
		SetWeight(7000.f);
		SetHp(64000.f);
		SetForce(840000.f);
		break;
	case E_ModuleLevel_ca::Hotel:
		SetWeight(8000.f);
		SetHp(128000.f);
		SetForce(960000.f);
		break;
	case E_ModuleLevel_ca::India:
		SetWeight(9000.f);
		SetHp(256000.f);
		SetForce(1080000.f);
		break;
	case E_ModuleLevel_ca::Juliet:
		SetWeight(10000.f);
		SetHp(512000.f);
		SetForce(1200000.f);
		break;
	case E_ModuleLevel_ca::Kilo:
		SetWeight(11000.f);
		SetHp(1024000.f);
		SetForce(1200000.f);
		break;
	default:
		assert(nullptr);
		break;
	}
}