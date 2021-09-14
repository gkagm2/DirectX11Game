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
	m_pParentModuleConnector{nullptr},
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

void CModuleScript_ca::ConnectModule(TModuleConnector_ca& _tOtherConnector)
{
	assert(true == MainConnector().bIsConnectable);
	assert(true == _tOtherConnector.bIsConnectable);
	MainConnector().bIsConnectable = false;
	_tOtherConnector.bIsConnectable = false;
	m_pParentModuleConnector = &_tOtherConnector;
}

void CModuleScript_ca::DisConnectModule()
{
	assert(m_pParentModuleConnector);
	assert(false == MainConnector().bIsConnectable);
	assert(false == m_pParentModuleConnector->bIsConnectable);
	MainConnector().bIsConnectable = true;
	m_pParentModuleConnector->bIsConnectable = true;
	m_pParentModuleConnector = nullptr;
}

void CModuleScript_ca::Update()
{
}

bool CModuleScript_ca::SaveToScene(FILE* _pFile)
{
	FWrite(m_eModuleLevel, _pFile);
	FWrite(m_eModuleType, _pFile);
	FWrite(m_eModuleSize, _pFile);
	return true;
}
bool CModuleScript_ca::LoadFromScene(FILE* _pFile)
{
	FRead(m_eModuleLevel, _pFile);
	FRead(m_eModuleType, _pFile);
	FRead(m_eModuleSize, _pFile);

	SetModuleLevel(m_eModuleLevel);
	SetModuleSize(m_eModuleSize);
	return true;
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

TModuleConnector_ca& CModuleScript_ca::MainConnector()
{
	vector<TModuleConnector_ca>& vecConnectors = GetConnectors();
	for (UINT i = 0; i < vecConnectors.size(); ++i) {
		if (vecConnectors[i].bIsMain) {
			return vecConnectors[i];
		}
	}
	assert(nullptr);
	return vecConnectors[0];
}

const Vector3& CModuleScript_ca::GetMainConnectionLocalPosition()
{
	Vector3 vMainLocalPosition = {};
	const vector<TModuleConnector_ca>& vecConnectPoints = GetConnectors();
	for (UINT i = 0; i < vecConnectPoints.size(); ++i) {
		if (vecConnectPoints[i].bIsMain) {
			vMainLocalPosition = vecConnectPoints[i].vPosition;
			break;
		}
	}
	return vMainLocalPosition;
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
		tConnectPoint.eDirection = E_Direction_ca::Back;
		AddModuleConnectPoint(tConnectPoint);

		//(아래) 아래 -> 위
		tConnectPoint.vDirection = Vector3(0.f, 1.f, 0.f);
		tConnectPoint.vPosition = Vector3(0.f, -0.5f, 0.f);
		tConnectPoint.bIsMain = true;
		tConnectPoint.bIsConnectable = true;
		tConnectPoint.eDirection = E_Direction_ca::Forward; // Main Connector를 Forward 방향으로 지정
		AddModuleConnectPoint(tConnectPoint);

		//(왼쪽) 왼쪽->오른쪽
		tConnectPoint.vDirection = Vector3(-1.f, 0.f, 0.f);
		tConnectPoint.vPosition = Vector3(-0.5f, 0.f, 0.f);
		tConnectPoint.bIsMain = false;
		tConnectPoint.bIsConnectable = true;
		tConnectPoint.eDirection = E_Direction_ca::Right;
		AddModuleConnectPoint(tConnectPoint);

		//(오른쪽) 오른쪽 -> 왼쪽
		tConnectPoint.vDirection = Vector3(1.f, 0.f, 0.f);
		tConnectPoint.vPosition = Vector3(0.5f, 0.f, 0.f);
		tConnectPoint.bIsMain = false;
		tConnectPoint.bIsConnectable = true;
		tConnectPoint.eDirection = E_Direction_ca::Left;
		AddModuleConnectPoint(tConnectPoint);
		break;
	}
		
	case E_ModuleSize_ca::Size1x2: {
		//(위) 위 -> 아래
		TModuleConnector_ca tConnectPoint;
		tConnectPoint.vDirection = Vector3(0.f, -1.f, 0.f);
		tConnectPoint.vPosition = Vector3(0.f, 0.1f, 0.f);
		tConnectPoint.bIsMain = false;
		tConnectPoint.bIsConnectable = true;
		tConnectPoint.eDirection = E_Direction_ca::Back;
		AddModuleConnectPoint(tConnectPoint);

		//(아래) 아래 -> 위
		tConnectPoint.vDirection = Vector3(0.f, 1.f, 0.f);
		tConnectPoint.vPosition = Vector3(0.f, -0.1f, 0.f);
		tConnectPoint.bIsMain = true;
		tConnectPoint.bIsConnectable = true;
		tConnectPoint.eDirection = E_Direction_ca::Forward;
		AddModuleConnectPoint(tConnectPoint);

		//(왼쪽 상단) 왼쪽->오른쪽
		tConnectPoint.vDirection = Vector3(-1.f, 0.f, 0.f);
		tConnectPoint.vPosition = Vector3(-0.5f, 0.5f, 0.f);
		tConnectPoint.bIsMain = false;
		tConnectPoint.bIsConnectable = true;
		tConnectPoint.eDirection = E_Direction_ca::Right;
		AddModuleConnectPoint(tConnectPoint);

		//(왼쪽 하단) 왼쪽->오른쪽
		tConnectPoint.vDirection = Vector3(-1.f, 0.f, 0.f);
		tConnectPoint.vPosition = Vector3(-0.5f, -0.5f, 0.f);
		tConnectPoint.bIsMain = false;
		tConnectPoint.bIsConnectable = true;
		tConnectPoint.eDirection = E_Direction_ca::Right;
		AddModuleConnectPoint(tConnectPoint);

		//(오른쪽 상단) 오른쪽 -> 왼쪽
		tConnectPoint.vDirection = Vector3(1.f, 0.f, 0.f);
		tConnectPoint.vPosition = Vector3(0.5f, 0.5f, 0.f);
		tConnectPoint.bIsMain = false;
		tConnectPoint.bIsConnectable = true;
		tConnectPoint.eDirection = E_Direction_ca::Left;
		AddModuleConnectPoint(tConnectPoint);

		//(오른쪽 하단) 오른쪽 -> 왼쪽
		tConnectPoint.vDirection = Vector3(1.f, 0.f, 0.f);
		tConnectPoint.vPosition = Vector3(0.5f, -0.5f, 0.f);
		tConnectPoint.bIsMain = false;
		tConnectPoint.bIsConnectable = true;
		tConnectPoint.eDirection = E_Direction_ca::Left;
		AddModuleConnectPoint(tConnectPoint);
		break;
	}
	case E_ModuleSize_ca::Size_OnlyMain: {
		//(아래) 아래 -> 위
		TModuleConnector_ca tConnectPoint;
		tConnectPoint.vDirection = Vector3(0.f, 1.f, 0.f);
		tConnectPoint.vPosition = Vector3(0.f, -1.f, 0.f);
		tConnectPoint.bIsMain = true;
		tConnectPoint.bIsConnectable = true;
		tConnectPoint.eDirection = E_Direction_ca::Forward;
		AddModuleConnectPoint(tConnectPoint);
	}
		break;
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