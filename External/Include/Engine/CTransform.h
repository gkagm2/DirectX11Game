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
	Vector3 GetPosition() { return m_matWorld.Translation(); }
	
	void SetLocalScale(const Vector3& _vScale) { m_vLocalScale = _vScale; }
	const Vector3& GetLocalScale() const { return m_vLocalScale; }
	Vector3 GetScale();

	// Radian���� �Է¹���
	void SetLocalRotation(const Vector3& _vRotation) { m_vLocalRotation = _vRotation; }
	const Vector3& GetLocalRotation() const { return m_vLocalRotation; }

	const Matrix& GetWorldMatrix() { return m_matWorld; }
	const Matrix& GetLocalMatrix4x4() { return m_matLocal; }

public:
	virtual void FinalUpdate() override;
	virtual void UpdateData() override;
	
public:
	CLONE(CTransform);
	CTransform();
	virtual ~CTransform() override;
};