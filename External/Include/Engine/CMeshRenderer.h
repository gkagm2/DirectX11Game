#pragma once
#include "CRenderer.h"
#include "CMesh.h"
#include "CMaterial.h"

class CMeshRenderer : public CRenderer
{
private:
	SharedPtr<CMesh>			m_pMesh;
	SharedPtr<CMaterial>		m_pMtrl;

	UINT m_iColor;

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

	void SetColor(BYTE _r, BYTE _g, BYTE _b, BYTE _a) {
		m_iColor = COLOR_RGBA(_r, _g, _b, _a);
	}
	UINT GetColor() { return m_iColor; }

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;
public:
	CLONE(CMeshRenderer);
	CMeshRenderer();
	virtual ~CMeshRenderer() override;
};