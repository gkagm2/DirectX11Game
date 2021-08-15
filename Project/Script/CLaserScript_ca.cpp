#include "pch.h"
#include "CLaserScript_ca.h"

CLaserScript_ca::CLaserScript_ca() :
	CModuleScript_ca((UINT)SCRIPT_TYPE::LASERSCRIPT_CA),
	m_fDamage(1.f),
	m_fFireRate(1.f)
{
}

CLaserScript_ca::~CLaserScript_ca()
{
}

void CLaserScript_ca::Start()
{
}

void CLaserScript_ca::Update()
{
}

void CLaserScript_ca::InitModule(E_ModuleLevel_ca _eLevel)
{
	switch (_eLevel) {
	case E_ModuleLevel_ca::Alpha:
		SetWeight(1000.f);
		SetHp(500.f);
		SetDamage(500.f);
		SetFireRate(1.f);
		break;
	case E_ModuleLevel_ca::Bravo:
		SetWeight(2000.f);
		SetHp(1000.f);
		SetDamage(500.f);
		SetFireRate(2.f);
		break;
	case E_ModuleLevel_ca::Charlie:
		SetWeight(3000.f);
		SetHp(2000.f);
		SetDamage(500.f);
		SetFireRate(4.f);
		break;
	case E_ModuleLevel_ca::Delta:
		SetWeight(4000.f);
		SetHp(4000.f);
		SetDamage(4000.f);
		SetFireRate(1.f);
		break;
	case E_ModuleLevel_ca::Echo:
		SetWeight(5000.f);
		SetHp(8000.f);
		SetDamage(4000.f);
		SetFireRate(2.f);
		break;
	case E_ModuleLevel_ca::Foxtrot:
		SetWeight(6000.f);
		SetHp(16000.f);
		SetDamage(4000.f);
		SetFireRate(4.f);
		break;
	case E_ModuleLevel_ca::Golf:
		SetWeight(7000.f);
		SetHp(32000.f);
		SetDamage(32000.f);
		SetFireRate(1.f);
		break;
	case E_ModuleLevel_ca::Hotel:
		SetWeight(8000.f);
		SetHp(64000.f);
		SetDamage(32000.f);
		SetFireRate(2.f);
		break;
	case E_ModuleLevel_ca::India:
		SetWeight(9000.f);
		SetHp(128000.f);
		SetDamage(32000.f);
		SetFireRate(4.f);
		break;
	case E_ModuleLevel_ca::Juliet:
		SetWeight(10000.f);
		SetHp(256000.f);
		SetDamage(256000.f);
		SetFireRate(1.f);
		break;
	case E_ModuleLevel_ca::Kilo:
		SetWeight(11000.f);
		SetHp(512000.f);
		SetDamage(256000.f);
		SetFireRate(2.f);
		break;
	default:
		assert(nullptr);
		break;
	}
}
