#pragma once
#include "CRenderer.h"
class CDecal : public CRenderer
{
private:
	SharedPtr<CMesh> m_pMesh;
	SharedPtr<CMaterial> m_pDecalMtrl;
	SharedPtr<CMaterial> m_pDebugMtrl;

public:
	virtual void FinalUpdate() override;
	virtual void Render() override;
	virtual void UpdateData();

public:
	SharedPtr<CMesh> GetMesh() { return m_pMesh; }
	void SetMesh(SharedPtr<CMesh> _pMesh) { m_pMesh = _pMesh; }
	SharedPtr<CMaterial> GetMaterial() { return m_pDecalMtrl; }
	void SetMaterial(SharedPtr<CMaterial> _pMtrl) { m_pDecalMtrl = _pMtrl; }
	SharedPtr<CMaterial>GetDebugMaterial() { return m_pDebugMtrl; }
	void SetDebugMaterial(SharedPtr<CMaterial> _pMtrl) { m_pDebugMtrl = _pMtrl; }

public:
	CLONE(CDecal);
	CDecal();
	virtual ~CDecal() override;
};