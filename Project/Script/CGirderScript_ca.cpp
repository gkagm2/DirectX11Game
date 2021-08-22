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
	InitModule(E_ModuleLevel_ca::Alpha);
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

	_ClearModuleConnectPoint();
	//(위) 위 -> 아래
	TModuleConnectPoint_ca tConnectPoint;
	tConnectPoint.vDirection = Vector3(0.f, -1.f, 0.f);
	tConnectPoint.vPosition = Vector3(0.f, 0.5f, 0.f);
	tConnectPoint.bIsMain = false;
	AddModuleConnectPoint(tConnectPoint);

	//(아래) 아래 -> 위
	tConnectPoint.vDirection = Vector3(0.f, 1.f, 0.f);
	tConnectPoint.vPosition = Vector3(0.f, -0.5f, 0.f);
	tConnectPoint.bIsMain = true;
	AddModuleConnectPoint(tConnectPoint);

	//(왼쪽) 왼쪽->오른쪽
	tConnectPoint.vDirection = Vector3(-1.f, 0.f, 0.f);
	tConnectPoint.vPosition = Vector3(-0.5f, 0.f, 0.f);
	tConnectPoint.bIsMain = false;
	AddModuleConnectPoint(tConnectPoint);

	//(오른쪽) 오른쪽 -> 왼쪽
	tConnectPoint.vDirection = Vector3(1.f, 0.f, 0.f);
	tConnectPoint.vPosition = Vector3(0.5f, 0.f, 0.f);
	tConnectPoint.bIsMain = false;
	AddModuleConnectPoint(tConnectPoint);
}
