#pragma once
#include "CComponent.h"
class CTransform : public CComponent
{
private:
	Vector3 m_vLocalPosition;
	Vector3 m_vLocalScale;
	Vector3 m_vLocalRotation;

	Matrix m_matLocal;
	Matrix m_matWorld;			// transform�� ���������� ���� ���

public:
	void SetLocalPosition(const Vector3& _vPosition) { m_vLocalPosition = _vPosition;	}
	const Vector3& GetLocalPosition() const { return m_vLocalPosition; }

	const Vector3& GetPosition() const { return Vector3(m_matWorld._41, m_matWorld._42, m_matWorld._43); }
	
	void SetLocalScale(const Vector3& _vScale) { m_vLocalScale = _vScale; }
	const Vector3& GetLocalScale() const { return m_vLocalScale; }
	const Vector3& GetScale() const { return Vector3(m_matWorld._11, m_matWorld._22, m_matWorld._33); }

	// Radian���� �Է¹���
	void SetLocalRotation(const Vector3& _vRotation) { m_vLocalRotation = _vRotation; }
	const Vector3& GetLocalRotation() const { return m_vLocalRotation; }

	const Matrix& GetWorldMatrix4x4() { return m_matWorld; }
	const Matrix& GetLocalMatrix4x4() { return m_matLocal; }

public:
	virtual void FinalUpdate() override;
	virtual void UpdateData() override;
	
public:
	CTransform();
	virtual ~CTransform() override;
};