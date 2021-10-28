#pragma once
#include "CRenderer.h"
class CSpriteRenderer : public CRenderer
{
private:
	SharedPtr<CMesh> m_pMesh;
	SharedPtr<CMaterial> m_pMtrl;
	SharedPtr<CMaterial> m_pCloneMaterial;
	SharedPtr<CMaterial> m_pSharedMaterial;

public:
	SharedPtr<CMaterial> GetCloneMaterial();
	SharedPtr<CMaterial> GetSharedMaterial() { return m_pMtrl; }

public:
	virtual void Render() override;

public:
	CLONE(CSpriteRenderer);
	CSpriteRenderer();
	virtual ~CSpriteRenderer() override;
};