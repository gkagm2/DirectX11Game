#pragma once
#include "CRenderer.h"
#include "CMesh.h"
#include "CMaterial.h"

class CMeshRenderer : public CRenderer
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

public:
	CLONE(CMeshRenderer);
	CMeshRenderer();
	virtual ~CMeshRenderer() override;
};