#pragma once
#include "CRenderer.h"
#include "CTexture.h"

class CMesh;
class CGraphicsShader;
class CMeshRenderer : public CRenderer
{
private:
	CMesh*			 m_pMesh;
	CGraphicsShader* m_pShader;
	SharedPtr<CTexture>	 m_pTexture;

public:
	virtual void Render() override;

public:
	void SetMeshRenderer(CMesh* _pMesh, CGraphicsShader* _pShader) { m_pMesh = _pMesh; m_pShader = _pShader; }

	void SetMesh(CMesh* _pMesh) { m_pMesh = _pMesh; }
	void SetShader(CGraphicsShader* _pShader) { m_pShader = _pShader; }
	void SetTexture(SharedPtr<CTexture> _pTexture) {	m_pTexture = _pTexture;	}

public:
	CMesh* GetMesh() { return m_pMesh; }
	CGraphicsShader* GetShader() { return m_pShader; }

public:
	CMeshRenderer();
	virtual ~CMeshRenderer() override;
};