#pragma once
#include "CCollider.h"

class CCollider2D : public CCollider
{
protected:
	Vector2 m_vOffsetPosition;
	Vector2 m_vOffsetScale;
	Matrix m_matColWorld; // Col : Collider

	SharedPtr<CMesh> m_pMesh;
	SharedPtr<CMaterial> m_pMaterial;

public:
	virtual void Render() override;
	virtual void FinalUpdate() override;

private:
	virtual void UpdateData() override;

public:
	virtual void OnCollisionEnter(CCollider2D* _pOther);
	virtual void OnCollisionStay(CCollider2D* _pOther);
	virtual void OnCollisionExit(CCollider2D* _pOther);
	
protected:
	void SetMesh(const SharedPtr<CMesh>& _pMesh) { m_pMesh = _pMesh; }
	void SetMaterial(const SharedPtr<CMaterial> _pMaterial) { m_pMaterial = _pMaterial; }

public:
	const Vector2& GetOffsetPosition() { return m_vOffsetPosition; }
	void SetOffsetPosition(const Vector2& _vPosition) { m_vOffsetPosition = _vPosition; }

	const Vector2& GetOffsetScale() { return m_vOffsetScale; }
	void SetOffsetScale(const Vector2& _vScale) { m_vOffsetScale = _vScale; }

	const Matrix& GetWorldMatrix() { return m_matColWorld; }

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CCollider2D);
	CCollider2D();
	virtual ~CCollider2D() override;
};