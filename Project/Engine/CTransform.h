#pragma once
#include "CComponent.h"
class CTransform : public CComponent
{
protected:
	// MeshRenderer���� �̿�
	Vector3 m_vLocalPosition;
	Vector3 m_vLocalScale;
	Vector3 m_vLocalRotation;	// Radian��

	Vector3 m_vLocalUpDir;
	Vector3 m_vLocalFrontDir;
	Vector3 m_vLocalRightDir;

	Vector3 m_vWorldUpDir;
	Vector3 m_vWorldFrontDir;
	Vector3 m_vWorldRightDir;

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
	Vector3 GetRotation();

	// Degree�� �Է¹���
	void SetLocalRotationDegree(const Vector3& _vDegree) {
		m_vLocalRotation = _vDegree* CMyMath::Deg2Rad();
	}
	Vector3 GetLocalRotationDegree() {
		return m_vLocalRotation * CMyMath::Rad2Deg();
	}
	Vector3 GetRotationDegree();

	// ȸ��
	void RotateLocalXDegree(float _fDegree) { m_vLocalRotation.x += _fDegree * CMyMath::Rad2Deg(); }
	void RotateLocalYDegree(float _fDegree) { m_vLocalRotation.y += _fDegree * CMyMath::Rad2Deg(); }
	void RotateLocalZDegree(float _fDegree) { m_vLocalRotation.z += _fDegree * CMyMath::Rad2Deg(); }
	void RotateLocalX(float _fAngle) { m_vLocalRotation.x += _fAngle; }
	void RotateLocalY(float _fAngle) { m_vLocalRotation.y += _fAngle; }
	void RotateLocalZ(float _fAngle) { m_vLocalRotation.z += _fAngle; }

	const Matrix& GetWorldMatrix() { return m_matWorld; }
	const Matrix& GetLocalMatrix4x4() { return m_matLocal; }


	const Vector3& GetRightVector() { return m_vWorldRightDir; }
	const Vector3& GetUpVector() { return m_vWorldUpDir; }
	const Vector3& GetFrontVector() { return m_vWorldFrontDir; }

	const Vector3& GetLocalRightVector() { return m_vLocalRightDir; }
	const Vector3& GetLocalUpVector() {	return m_vLocalUpDir; }
	const Vector3& GetLocalFrontVector() { return m_vLocalFrontDir; }

protected:
	// CanvasRenderer���� �̿�
	// (RectTransform, 2D Sprite����)
	float m_fWidth;
	float m_fHeight;
	Matrix m_matLocal_noParentScale;
	Matrix m_matWorld_noParentScale; // �θ� �ڽİ� scale�� ������� ���� ���
	Vector3 m_vLocalScale_RT; // Rect Transform���� ����ϴ� width, height �� �ݿ��� scale ��

public:
	// RectTransform, 2D Sprite���� �̿� (Canvas Material �̿�)
	void SetWidth(float _fWidth) { m_fWidth = _fWidth; }
	void SetHeight(float _fHeight) { m_fHeight = _fHeight; }
	float GetWidth() { return m_fWidth; }
	float GetHeight() { return m_fHeight; }

	const Matrix& GetWorldMatrix_NoParentScale() { return m_matWorld_noParentScale; }
	const Matrix& GetLocalMatrix_NoParentScale() { return m_matLocal_noParentScale; }

	Vector3 GetLocalScale_RectTR() { return m_vLocalScale_RT; }

public:
	void LookAt2D(const Vector2& m_vWorldPos);

public:
	void _LinkParent();
	void _UnlinkParent(const Vector3& vParentLocalScale, const Vector3& vParentLocalRotation);

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