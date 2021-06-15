#pragma once
#include "CCollider.h"

class CCollider2D : public CCollider
{
protected:
	Vector2 m_vOffsetPosition;
	Vector2 m_vOffsetScale;
	Matrix m_matWorld;

	SharedPtr<CMesh> m_pMesh;
	SharedPtr<CMaterial> m_pMaterial;

public:
	virtual void Render() override;
	virtual void FinalUpdate() override;

private:
	virtual void UpdateData() override;
	
protected:
	void SetMesh(const SharedPtr<CMesh>& _pMesh) { m_pMesh = _pMesh; }
	void SetMaterial(const SharedPtr<CMaterial> _pMaterial) { m_pMaterial = _pMaterial; }

public:
	const Vector2& GetOffsetPosition() { return m_vOffsetPosition; }
	void SetOffsetPosition(const Vector2& _vPosition) { m_vOffsetPosition = _vPosition; }

	const Vector2& GetOffsetScale() { return m_vOffsetScale; }
	void SetOffsetScale(const Vector2& _vScale) { m_vOffsetScale = _vScale; }

public:
	CCollider2D();
	virtual ~CCollider2D() override;
};