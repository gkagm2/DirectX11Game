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