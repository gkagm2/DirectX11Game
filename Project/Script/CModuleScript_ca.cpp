#include "pch.h"
#include "CModuleScript_ca.h"

const TCHAR* ModuleLevelSTR_ca[(UINT)E_ModuleLevel_ca::End] = {
	_T("Nemesis"),
	_T("Alpha"),
	_T("Bravo"),
	_T("Charlie"),
	_T("Delta"),
	_T("Echo"),
	_T("Foxtrot"),
	_T("Golf"),
	_T("Hotel"),
	_T("India"),
	_T("Juliet"),
	_T("Kilo")
};
const TCHAR* ModuleTypeSTR_ca[(UINT)E_ModuleType_ca::End] = {
	_T("Girder1x1"),
	_T("Girder1x2"),
	_T("Laser"),
	_T("Booster"),
	_T("Command")
}; 

CModuleScript_ca::CModuleScript_ca() :
	CScript((UINT)SCRIPT_TYPE::MODULESCRIPT_CA),
	m_pParentModuleGameObj(nullptr),
	m_eModuleLevel(E_ModuleLevel_ca::Alpha),
	m_eModuleType(E_ModuleType_ca::Girder1x1),
	m_eModuleSize(E_ModuleSize_ca::Size1x1),
	m_fHp(1.f),
	m_fWeight(1.f)
{
	assert(nullptr);
}

CModuleScript_ca::CModuleScript_ca(UINT _iScriptNum, E_ModuleType_ca _eModuleType, E_ModuleLevel_ca _eModuleLevel, E_ModuleSize_ca _eModuleSize) :
	CScript(_iScriptNum),
	m_pParentModuleGameObj(nullptr),
	m_eModuleLevel(_eModuleLevel),
	m_eModuleType(_eModuleType),
	m_eModuleSize(_eModuleSize),
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

const Vector3& CModuleScript_ca::FindNearestConnectionPosition(const Vector3& _vPosition)
{
	const vector<TModuleConnector_ca>& vecConnectors = GetConnectors();
	Vector3 vNearestPos = {};
	float fMinDistance = (numeric_limits<float>::max)();
	for (UINT i = 0; i < vecConnectors.size(); ++i) {
		Vector3 vWorldPos = XMVector3TransformCoord(vecConnectors[i].vPosition, Transform()->GetWorldMatrix());
		float fDis = Vector3::Distance(_vPosition, vWorldPos);
		if (fMinDistance > fDis) {
			vNearestPos = vWorldPos;
			fMinDistance = fDis;
		}
	}
	return vNearestPos;
}

TModuleConnector_ca& CModuleScript_ca::FindNearestConnector(const Vector3& _vPosition)
{
	vector<TModuleConnector_ca>& vecConnectors = GetConnectors();
	Vector3 vNearestPos = {};
	float fMinDistance = (numeric_limits<float>::max)();
	UINT idx = 0;
	for (UINT i = 0; i < vecConnectors.size(); ++i) {
		Vector3 vWorldPos = XMVector3TransformCoord(vecConnectors[i].vPosition, Transform()->GetWorldMatrix());
		float fDis = Vector3::Distance(_vPosition, vWorldPos);
		if (fMinDistance > fDis) {
			vNearestPos = vWorldPos;
			fMinDistance = fDis;
			idx = i;
		}
	}

	return vecConnectors[idx];
}

const Vector3& CModuleScript_ca::GetMainConnectionPosition()
{
	Vector3 vMainPosition = {};
	const vector<TModuleConnector_ca>& vecConnectPoints = GetConnectors();
	for (UINT i = 0; i < vecConnectPoints.size(); ++i) {
		if (vecConnectPoints[i].bIsMain) {
			vMainPosition = XMVector3TransformCoord(vecConnectPoints[i].vPosition, Transform()->GetWorldMatrix());
			break;
		}
	}
	return vMainPosition;
}

void CModuleScript_ca::_InitModuleSize(E_ModuleSize_ca _eModuleSize)
{
	_ClearModuleConnectPoint();
	switch (_eModuleSize) {
	case E_ModuleSize_ca::Size1x1: {
		//(위) 위 -> 아래
		TModuleConnector_ca tConnectPoint;
		tConnectPoint.vDirection = Vector3(0.f, -1.f, 0.f);
		tConnectPoint.vPosition = Vector3(0.f, 0.5f, 0.f);
		tConnectPoint.bIsMain = false;
		tConnectPoint.bIsConnectable = true;
		AddModuleConnectPoint(tConnectPoint);

		//(아래) 아래 -> 위
		tConnectPoint.vDirection = Vector3(0.f, 1.f, 0.f);
		tConnectPoint.vPosition = Vector3(0.f, -0.5f, 0.f);
		tConnectPoint.bIsMain = true;
		tConnectPoint.bIsConnectable = true;
		AddModuleConnectPoint(tConnectPoint);

		//(왼쪽) 왼쪽->오른쪽
		tConnectPoint.vDirection = Vector3(-1.f, 0.f, 0.f);
		tConnectPoint.vPosition = Vector3(-0.5f, 0.f, 0.f);
		tConnectPoint.bIsMain = false;
		tConnectPoint.bIsConnectable = true;
		AddModuleConnectPoint(tConnectPoint);

		//(오른쪽) 오른쪽 -> 왼쪽
		tConnectPoint.vDirection = Vector3(1.f, 0.f, 0.f);
		tConnectPoint.vPosition = Vector3(0.5f, 0.f, 0.f);
		tConnectPoint.bIsMain = false;
		tConnectPoint.bIsConnectable = true;
		AddModuleConnectPoint(tConnectPoint);
		break;
	}
		
	case E_ModuleSize_ca::Size1x2: {
		//(위) 위 -> 아래
		TModuleConnector_ca tConnectPoint;
		tConnectPoint.vDirection = Vector3(0.f, -1.f, 0.f);
		tConnectPoint.vPosition = Vector3(0.f, 0.5f, 0.f);
		tConnectPoint.bIsMain = false;
		tConnectPoint.bIsConnectable = true;
		AddModuleConnectPoint(tConnectPoint);

		//(아래) 아래 -> 위
		tConnectPoint.vDirection = Vector3(0.f, 1.f, 0.f);
		tConnectPoint.vPosition = Vector3(0.f, -0.5f, 0.f);
		tConnectPoint.bIsMain = true;
		tConnectPoint.bIsConnectable = true;
		AddModuleConnectPoint(tConnectPoint);

		//(왼쪽 상단) 왼쪽->오른쪽
		tConnectPoint.vDirection = Vector3(-1.f, 0.f, 0.f);
		tConnectPoint.vPosition = Vector3(-0.5f, 0.125f, 0.f);
		tConnectPoint.bIsMain = false;
		tConnectPoint.bIsConnectable = true;
		AddModuleConnectPoint(tConnectPoint);

		//(왼쪽 하단) 왼쪽->오른쪽
		tConnectPoint.vDirection = Vector3(-1.f, 0.f, 0.f);
		tConnectPoint.vPosition = Vector3(-0.5f, -0.125f, 0.f);
		tConnectPoint.bIsMain = false;
		tConnectPoint.bIsConnectable = true;
		AddModuleConnectPoint(tConnectPoint);

		//(오른쪽 상단) 오른쪽 -> 왼쪽
		tConnectPoint.vDirection = Vector3(1.f, 0.f, 0.f);
		tConnectPoint.vPosition = Vector3(0.5f, 0.125f, 0.f);
		tConnectPoint.bIsMain = false;
		tConnectPoint.bIsConnectable = true;
		AddModuleConnectPoint(tConnectPoint);

		//(오른쪽 하단) 오른쪽 -> 왼쪽
		tConnectPoint.vDirection = Vector3(1.f, 0.f, 0.f);
		tConnectPoint.vPosition = Vector3(0.5f, -0.125f, 0.f);
		tConnectPoint.bIsMain = false;
		tConnectPoint.bIsConnectable = true;
		AddModuleConnectPoint(tConnectPoint);
		break;
	}
	default:
		assert(nullptr);
		break;
	}
} 

void CModuleScript_ca::SetModuleLevel(E_ModuleLevel_ca _eModuleLevel) {
	m_eModuleLevel = _eModuleLevel;
	_InitModuleLevel(_eModuleLevel);
}
void CModuleScript_ca::SetModuleSize(E_ModuleSize_ca _eModuleSize) {
	m_eModuleSize = _eModuleSize;
	_InitModuleSize(_eModuleSize);
}