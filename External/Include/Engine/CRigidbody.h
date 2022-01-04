#pragma once
#include "CComponent.h"

class CGameObject;
class CRigidbody : public CComponent
{
private:
	Vector3 m_vForce; // ��
	Vector3 m_vAccel; // ���ӵ�
	float m_fMass;	// ����
	Vector3 m_vVelocity; // �ӵ�
	float m_fDrag; // ���� ����

	float m_fSpeed; // �ӷ�
	float m_fMaxSpeed; // �ִ� �ӷ�

	// TOOD : ������ ����� ������ �߰�
	float m_fFriction; // ������
	bool m_bIsFrictionActive; // ������ ����

	Vector3 m_vGravityAccel; // �߷� ���ӵ�
	bool m_bUseGravity; // �߷� ��� �÷���
	bool m_bIsKinematic; // ���� ȿ�� ���ֱ� �÷���


public:
	virtual void PrevUpdate();
	virtual void Update();
	virtual void LateUpdate();

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	void SetMaxSpeed(float _fMaxSpeed) { m_fMaxSpeed = _fMaxSpeed; } // �ִ� �ӵ�
	float GetMaxSpeed() { return m_fMaxSpeed; }
	void SetMass(float _fMass) { m_fMass = max(_fMass, 0.0f); } // ���� (0 ~ alpha)
	float GetMass() const { return m_fMass; }
	void SetVelocity(Vector3 _fVelocity) { m_vVelocity = _fVelocity; }
	const Vector3& GetVelocity() const { return m_vVelocity; }
	void SetDrag(float _fDrag) { m_fDrag = max(_fDrag, 0.0f); } // ���� ���� (0 ~ alpha)
	float GetDrag() const { return m_fDrag; }
	float GetSpeed() { return m_fSpeed; }
	// inline void SetAngularDrag(float _fAngularDrag); // ȸ������ ���� ����
	void UseGravity(bool _bUseGravity) { m_bUseGravity = _bUseGravity; } // �߷� ��� ���� ����
	bool IsOnGravity() const { return m_bUseGravity; }
	void IsKinematic(bool _bIsKinematic) { m_bIsKinematic = _bIsKinematic; } // ���� ȿ�� ���ֱ�
	bool IsOnKinematic() const { return m_bIsKinematic; }

public:
	void AddForce(Vector3 _vForce) { m_vForce += -_vForce; } // ���� �߰��ϴ�.
	void AddVelocity(Vector3 _vVelocity) { m_vVelocity += _vVelocity; } // �ӵ��� �߰��ϴ�.
	const Vector3& GetForce() { return m_vForce; } //read only

protected:
	CRigidbody(const CRigidbody& _other);

public:
	CLONE(CRigidbody);
	CRigidbody(E_ComponentType _eType);
	virtual ~CRigidbody() override;
};