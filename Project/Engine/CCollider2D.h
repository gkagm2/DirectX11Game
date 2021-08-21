#pragma once
#include "CCollider.h"

enum class E_Collider2DType {
	rect,
	circle
};

class CCollider2D : public CCollider, public ICollision2DInterface
{
public:
	
	struct TColliderType {
		E_Collider2DType eType;
		tstring strType;
	};
	
protected:
	TColliderType m_tColliderType; // 상속받은 클래스에서 반드시 초기화

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
	virtual void OnCollisionEnter2D(CCollider2D* _pOther) override;
	virtual void OnCollisionStay2D(CCollider2D* _pOther) override;
	virtual void OnCollisionExit2D(CCollider2D* _pOther) override;
	
protected:
	void SetMesh(const SharedPtr<CMesh>& _pMesh) { m_pMesh = _pMesh; }
	void SetMaterial(const SharedPtr<CMaterial> _pMaterial) { m_pMaterial = _pMaterial; }

public:
	E_Collider2DType GetColliderType() { return m_tColliderType.eType; }
	const tstring& GetColliderTypeName() { return m_tColliderType.strType; }

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