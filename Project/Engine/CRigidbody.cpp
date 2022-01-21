#include "pch.h"
#include "CRigidbody.h"
#include "CTimeManager.h"
#include "CComponent.h"
#include "CGameObject.h"
#include "CTransform.h"

CRigidbody::CRigidbody(E_ComponentType _eType) :
	CComponent(_eType),
	m_vForce{},
	m_vAccel{},
	m_fMass(0.5f),
	m_vVelocity{},
	m_fDrag(0.2f),
	m_fSpeed(0.f),
	m_fMaxSpeed(15.f),
	m_fFriction(0.f),
	m_bIsFrictionActive(true),
	m_vGravityAccel(0.0f, 0.98066f , 0.0f), // default (0.0f, 9.80665f, 0.0f)
	m_bUseGravity(false),
	m_bIsKinematic(false)
{
}

CRigidbody::CRigidbody(const CRigidbody& _other) :
	CComponent(_other),
	m_vForce{ _other.m_vForce },
	m_vAccel{ _other.m_vAccel},
	m_fMass( _other.m_fMass),
	m_vVelocity{ _other.m_vVelocity},
	m_fDrag( _other.m_fDrag),
	m_fSpeed( _other.m_fSpeed),
	m_fMaxSpeed( _other.m_fMaxSpeed),
	m_fFriction( _other.m_fFriction),
	m_bIsFrictionActive( _other.m_fFriction),
	m_vGravityAccel{ _other.m_vGravityAccel },
	m_bUseGravity( _other.m_bUseGravity),
	m_bIsKinematic( _other.m_bIsKinematic)
{
	// TODO : OwnerObject Setting
}

CRigidbody::~CRigidbody()
{
}

void CRigidbody::PrevUpdate()
{
	m_vForce = Vector3(0.f, 0.f, 0.f);
}

void CRigidbody::Update()
{
	if (nullptr == GetGameObject())
		return;
	if (m_bIsKinematic)
		return;
}

void CRigidbody::LateUpdate()
{
	if (nullptr == GetGameObject())
		return;
	if (m_bIsKinematic)
		return;

	if (0 == m_fMass)
		m_vAccel = {};
	else
		m_vAccel = m_vForce / m_fMass; // 가속도

		// 중력
	Vector3 vGravityAccel{};
	if (m_bUseGravity) { // f = m a = m 중력 가속도
		vGravityAccel = m_vGravityAccel;
		vGravityAccel.y *= -1;
		m_vAccel -= vGravityAccel;
	}

	m_vVelocity = m_vVelocity + (m_vAccel * DT - vGravityAccel * DT);

	// 공기 저항
	Vector3 vDragDir = m_vVelocity;
	vDragDir.Normalize();
	Vector3 vDragVec = -vDragDir * m_fDrag * DT;
	float fDragSpeed = vDragVec.Length();
	m_fSpeed = m_vVelocity.Length();
	if (fDragSpeed > m_fSpeed)
		m_vVelocity = Vector3(0.0f, 0.0f, 0.0f);
	else
		m_vVelocity += vDragVec;

	// 최대 속도 제한
	m_fSpeed = m_vVelocity.Length();
	if (m_fSpeed > m_fMaxSpeed * DT) {
		m_vVelocity.Normalize();
		m_vVelocity = m_vVelocity * m_fMaxSpeed * DT;
	}

	Vector3 vObjPos = GetGameObject()->Transform()->GetLocalPosition();
	GetGameObject()->Transform()->SetLocalPosition(vObjPos - m_vVelocity);
}

bool CRigidbody::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);
	FWrite(m_vForce, _pFile);
	FWrite(m_fMass, _pFile);
	FWrite(m_fDrag, _pFile);
	FWrite(m_fMaxSpeed, _pFile);
	FWrite(m_fFriction, _pFile);
	FWrite(m_bIsFrictionActive, _pFile);
	FWrite(m_vGravityAccel, _pFile);
	FWrite(m_bUseGravity, _pFile);
	FWrite(m_bIsKinematic, _pFile);

	return true;
}

bool CRigidbody::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);
	FRead(m_vForce, _pFile);
	FRead(m_fMass, _pFile);
	FRead(m_fDrag, _pFile);
	FRead(m_fMaxSpeed, _pFile);
	FRead(m_fFriction, _pFile);
	FRead(m_bIsFrictionActive, _pFile);
	FRead(m_vGravityAccel, _pFile);
	FRead(m_bUseGravity, _pFile);
	FRead(m_bIsKinematic, _pFile);

	return true;
}
