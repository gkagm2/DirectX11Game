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

const Vector3& CModuleScript_ca::FindNearestConnectionPosition(const Vector3& _vPositon)
{
	const vector<TModuleConnectPoint_ca>& vecConnectPoints = GetConnectPoints();
	Vector3 vNearestPos = {};
	float fMinDistance = (numeric_limits<float>::max)();
	for (UINT i = 0; i < vecConnectPoints.size(); ++i) {
		Vector3 vWorldPos = XMVector3TransformCoord(vecConnectPoints[i].vPosition, Transform()->GetWorldMatrix());
		float fDis = Vector3::Distance(_vPositon, vWorldPos);
		if (fMinDistance > fDis) {
			vNearestPos = vWorldPos;
			fMinDistance = fDis;
		}
	}
	return vNearestPos;
}

const Vector3& CModuleScript_ca::GetMainConnectionPosition()
{
	Vector3 vMainPosition = {};
	const vector<TModuleConnectPoint_ca>& vecConnectPoints = GetConnectPoints();
	for (UINT i = 0; i < vecConnectPoints.size(); ++i) {
		if (vecConnectPoints[i].bIsMain) {
			vMainPosition = XMVector3TransformCoord(vecConnectPoints[i].vPosition, Transform()->GetWorldMatrix());
			break;
		}
	}
	return vMainPosition;
}

void CModuleScript_ca::InitModuleSize(E_ModuleSize_ca _eModuleSize)
{
	_ClearModuleConnectPoint();
	switch (_eModuleSize) {
	case E_ModuleSize_ca::Size1x1: {
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
		break;
	}
		
	case E_ModuleSize_ca::Size1x2: {
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

		//(왼쪽 상단) 왼쪽->오른쪽
		tConnectPoint.vDirection = Vector3(-1.f, 0.f, 0.f);
		tConnectPoint.vPosition = Vector3(-0.5f, 0.125f, 0.f);
		tConnectPoint.bIsMain = false;
		AddModuleConnectPoint(tConnectPoint);

		//(왼쪽 하단) 왼쪽->오른쪽
		tConnectPoint.vDirection = Vector3(-1.f, 0.f, 0.f);
		tConnectPoint.vPosition = Vector3(-0.5f, -0.125f, 0.f);
		tConnectPoint.bIsMain = false;
		AddModuleConnectPoint(tConnectPoint);

		//(오른쪽 상단) 오른쪽 -> 왼쪽
		tConnectPoint.vDirection = Vector3(1.f, 0.f, 0.f);
		tConnectPoint.vPosition = Vector3(0.5f, 0.125f, 0.f);
		tConnectPoint.bIsMain = false;
		AddModuleConnectPoint(tConnectPoint);

		//(오른쪽 하단) 오른쪽 -> 왼쪽
		tConnectPoint.vDirection = Vector3(1.f, 0.f, 0.f);
		tConnectPoint.vPosition = Vector3(0.5f, -0.125f, 0.f);
		tConnectPoint.bIsMain = false;
		AddModuleConnectPoint(tConnectPoint);
		break;
	}
	default:
		assert(nullptr);
		break;
	}
}