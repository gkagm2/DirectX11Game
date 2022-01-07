#pragma once
#include "CComponent.h"

class CGameObject;
class CRigidbody : public CComponent
{
private:
	Vector3 m_vForce; // 힘
	Vector3 m_vAccel; // 가속도
	float m_fMass;	// 질량
	Vector3 m_vVelocity; // 속도
	float m_fDrag; // 공기 저항

	float m_fSpeed; // 속력
	float m_fMaxSpeed; // 최대 속력

	// TOOD : 접촉할 경우의 마찰력 추가
	float m_fFriction; // 마찰력
	bool m_bIsFrictionActive; // 마찰력 유무

	Vector3 m_vGravityAccel; // 중력 가속도
	bool m_bUseGravity; // 중력 사용 플래그
	bool m_bIsKinematic; // 물리 효과 없애기 플래그

public:
	virtual void PrevUpdate();
	virtual void Update();
	virtual void LateUpdate();

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	void SetMaxSpeed(float _fMaxSpeed) { m_fMaxSpeed = _fMaxSpeed; } // 최대 속도
	float GetMaxSpeed() { return m_fMaxSpeed; }
	void SetMass(float _fMass) { m_fMass = max(_fMass, 0.0f); } // 질량 (0 ~ alpha)
	float GetMass() const { return m_fMass; }
	void SetVelocity(Vector3 _fVelocity) { m_vVelocity = _fVelocity; }
	const Vector3& GetVelocity() const { return m_vVelocity; }
	void SetDrag(float _fDrag) { m_fDrag = max(_fDrag, 0.0f); } // 공기 저항 (0 ~ alpha)
	float GetDrag() const { return m_fDrag; }
	float GetSpeed() { return m_fSpeed; }
	// inline void SetAngularDrag(float _fAngularDrag); // 회전값에 대한 저항
	void UseGravity(bool _bUseGravity) { m_bUseGravity = _bUseGravity; } // 중력 사용 여부 설정
	bool IsOnGravity() const { return m_bUseGravity; }
	void IsKinematic(bool _bIsKinematic) { m_bIsKinematic = _bIsKinematic; } // 물리 효과 없애기
	bool IsOnKinematic() const { return m_bIsKinematic; }

public:
	void AddForce(Vector3 _vForce) { m_vForce += -_vForce; } // 힘을 추가하다.
	void AddVelocity(Vector3 _vVelocity) { m_vVelocity += _vVelocity; } // 속도를 추가하다.
	const Vector3& GetForce() { return m_vForce; } //read only

protected:
	CRigidbody(const CRigidbody& _other);

public:
	CLONE(CRigidbody);
	CRigidbody(E_ComponentType _eType);
	virtual ~CRigidbody() override;
};