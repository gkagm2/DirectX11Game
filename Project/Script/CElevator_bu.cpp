#include "pch.h"
#include "CElevator_bu.h"

CElevator_bu::CElevator_bu() :
	CInteractiveObj_bu((UINT)SCRIPT_TYPE::ELEVATOR_BU),
	m_fMoveTime(0.f),
	m_bDirFlag(true),
	m_pStartPosObj(nullptr),
	m_pEndPosObj(nullptr),
	m_fSpeed(0.2f),
	m_vStartPos{},
	m_vEndPos{},
	m_eType{ E_ElevationType::Once }
{
	AddParam(TScriptParam(_T("Move Speed"), E_ScriptParam::FLOAT, &m_fSpeed));
	AddParam(TScriptParam(_T("Type 0 : once, 1 : loop"), E_ScriptParam::BOOL, &m_eType));
}

CElevator_bu::CElevator_bu(const CElevator_bu& _origin) :
	CInteractiveObj_bu((UINT)SCRIPT_TYPE::ELEVATOR_BU),
	m_fMoveTime(_origin.m_fMoveTime),
	m_bDirFlag(_origin.m_bDirFlag),
	m_pStartPosObj(nullptr),
	m_pEndPosObj(nullptr),
	m_fSpeed(_origin.m_fSpeed),
	m_vStartPos{_origin.m_vStartPos},
	m_vEndPos{_origin.m_vEndPos},
	m_eType{_origin.m_eType}
{
	AddParam(TScriptParam(_T("Move Speed"), E_ScriptParam::FLOAT, &m_fSpeed));
	AddParam(TScriptParam(_T("Type 0 : once, 1 : loop"), E_ScriptParam::BOOL, &m_eType));
}

CElevator_bu::~CElevator_bu()
{
}

void CElevator_bu::Awake()
{
	m_pStartPosObj = GetGameObject()->FindGameObjectSameLine(_T("ElevatorStartPos"));
	m_pEndPosObj = GetGameObject()->FindGameObjectSameLine(_T("ElevatorEndPos"));
	assert(m_pStartPosObj);
	assert(m_pEndPosObj);
	m_vStartPos = m_pStartPosObj->Transform()->GetLocalPosition();
	m_vEndPos = m_pEndPosObj->Transform()->GetLocalPosition();
}

void CElevator_bu::Start()
{
}

void CElevator_bu::Update()
{
	if (m_eType == E_ElevationType::Once) {
		if (m_bDirFlag) {
			m_fMoveTime += DT * m_fSpeed;
		}
		else {
			m_fMoveTime -= DT * m_fSpeed;
		}
	}
	else if (m_eType == E_ElevationType::Loop) {
		m_fMoveTime += DT * m_fSpeed;
	}

	Vector3 vCurPos = Transform()->GetLocalPosition();
	
	if (m_eType == E_ElevationType::Loop) {
		float delta = fabsf(cosf(m_fMoveTime) + 1.f) * 0.5f;
		Vector3 vPos = CMyMath::Lerp(m_vStartPos, m_vEndPos, delta);
		Transform()->SetLocalPosition(vPos);
	}
	else if (m_eType == E_ElevationType::Once) {
		m_fMoveTime = CMyMath::Clamp(m_fMoveTime, 0.f, 1.f);
		Vector3 vPos = CMyMath::Lerp(m_vStartPos, m_vEndPos, m_fMoveTime);
		Transform()->SetLocalPosition(vPos);
		//CMyMath::Lerp(m_vEndPos, m_vStartPos, DT * m_fSpeed);
	}
	else
		assert(nullptr);
}

void CElevator_bu::Interaction(bool _bActive)
{
	m_bDirFlag = _bActive;
}

void CElevator_bu::Move()
{
	m_bIsActivate = true;
	m_bDirFlag = true;
}

void CElevator_bu::Stop()
{
	m_bIsActivate = false;
	m_bDirFlag = false;
}

bool CElevator_bu::SaveToScene(FILE* _pFile)
{
	FWrite(m_fSpeed, _pFile);
	return true;
}

bool CElevator_bu::LoadFromScene(FILE* _pFile)
{
	FRead(m_fSpeed, _pFile);
	return true;
}
