#pragma once
#include "CComponent.h"
class CTransform : public CComponent
{
private:
	Vector3 m_vLocalPosition;
	Vector3 m_vLocalScale;
	Vector3 m_vLocalRotation;

	Matrix m_matWorld;			// transform의 상태정보를 담은 행렬

public:
	void SetLocalPosition(const Vector3& _vPosition) { m_vLocalPosition = _vPosition;	}
	const Vector3& GetLocalPosition() const { return m_vLocalPosition; }
	
	void SetLocalScale(const Vector3& _vScale) { m_vLocalScale = _vScale; }
	const Vector3& GetLocalScale() const { return m_vLocalScale; }

	// Radian으로 입력받음
	void SetLocalRotation(const Vector3& _vRotation) { m_vLocalRotation = _vRotation; }
	const Vector3& GetLocalRotation() const { return m_vLocalRotation; }

public:
	virtual void LateUpdate() override;
	virtual void UpdateData() override;
	
public:
	CTransform();
	virtual ~CTransform() override;
};