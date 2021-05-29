#pragma once
#include "CRenderer.h"

class CMesh;
class CGraphicsShader;
class CMeshRenderer : public CRenderer
{
private:
	CMesh* m_pMesh;
	CGraphicsShader* m_pShader;

public:
	virtual void Render() override;

public:
	void SetMeshRenderer(CMesh* _pMesh, CGraphicsShader* _pShader) { m_pMesh = _pMesh; m_pShader = _pShader; }

protected:
	void SetMesh(CMesh* _pMesh) { m_pMesh = _pMesh; }
	void SetShader(CGraphicsShader* _pShader) { m_pShader = _pShader; }

public:
	CMesh* GetMesh() { return m_pMesh; }
	CGraphicsShader* GetShader() { return m_pShader; }

public:
	CMeshRenderer();
	virtual ~CMeshRenderer() override;
};