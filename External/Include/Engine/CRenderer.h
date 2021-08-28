#pragma once
#include "CComponent.h"
class CRenderer : public CComponent
{
private:
	SharedPtr<CMesh>			m_pMesh;
	SharedPtr<CMaterial>		m_pMtrl;

public:
	virtual void Render() override;

public:
	void SetMesh(SharedPtr<CMesh> _pMesh) { m_pMesh = _pMesh; }
	void SetMaterial(SharedPtr<CMaterial> _pMtrl) { m_pMtrl = _pMtrl; }

	SharedPtr<CMesh> GetMesh() { return m_pMesh; }
	SharedPtr<CMaterial> GetCloneMaterial();
	SharedPtr<CMaterial> GetSharedMaterial() { return m_pMtrl; }

	void SetTexSize(const Vector2& _vSize);
	const Vector2& GetTexSize();

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CRenderer);
	CRenderer() = delete;
	CRenderer(E_ComponentType _eType);
	virtual ~CRenderer() override;
};