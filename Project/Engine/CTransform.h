#pragma once
#include "CComponent.h"
class CTransform : public CComponent
{
protected:
	Vector3 m_vLocalPosition;
	Vector3 m_vLocalScale;
	Vector3 m_vLocalRotation; // Radian임

	Matrix m_matLocal;
	Matrix m_matWorld;			// transform의 상태정보를 담은 행렬

	Matrix m_matWorld_noParentScale; // 부모 자식간 scale을 고려하지 않은 행렬


public:
	void SetLocalPosition(const Vector3& _vPosition) { m_vLocalPosition = _vPosition;	}
	const Vector3& GetLocalPosition() const { return m_vLocalPosition; }
	Vector3 GetPosition() { return m_matWorld.Translation(); }
	
	void SetLocalScale(const Vector3& _vScale) { m_vLocalScale = _vScale; }
	const Vector3& GetLocalScale() const { return m_vLocalScale; }
	Vector3 GetScale();

	// Radian으로 입력받음
	void SetLocalRotation(const Vector3& _vRotation) { m_vLocalRotation = _vRotation; }
	const Vector3& GetLocalRotation() const { return m_vLocalRotation; }
	// Degree로 입력받음
	void SetLocalRotationDegree(const Vector3& _vDegree) {
		m_vLocalRotation = _vDegree* CMyMath::Deg2Rad();
	}
	const Vector3& GetLocalRotationDegree() {
		return m_vLocalRotation * CMyMath::Rad2Deg();
	}

	const Matrix& GetWorldMatrix() { return m_matWorld; }
	const Matrix& GetLocalMatrix4x4() { return m_matLocal; }
	const Matrix& GetWOrldMatrix_NoParentScale() { return m_matWorld_noParentScale; }

	const Vector3& GetUpVector() {
		Vector3 vUpVec = XMVector3TransformCoord(Vector3{ 0.f,1.f,0.f }, GetWorldMatrix());
		return  vUpVec.Normalize();
	}

public:
	virtual void FinalUpdate() override;
	virtual void UpdateData() override;



public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;
	
public:
	CLONE(CTransform);
	CTransform();
	CTransform(E_ComponentType _eComponentType);
	virtual ~CTransform() override;
};