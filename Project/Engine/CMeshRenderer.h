#pragma once
#include "CRenderer.h"

class CMesh;
class CShader;
class CGraphicsShader;
class CMeshRenderer : public CRenderer
{
private:
	CMesh* m_pMesh;
	CGraphicsShader* m_pShader;

public:
	virtual void Render() override;

public:
	void SetMesh(CMesh* _pMesh) { m_pMesh = _pMesh; }
	CMesh* GetMesh() { return m_pMesh; }

	void SetShader(CGraphicsShader* _pShader) { m_pShader = _pShader; }
	CGraphicsShader* GetShader() { return m_pShader; }

public:
	CMeshRenderer();
	virtual ~CMeshRenderer() override;
};