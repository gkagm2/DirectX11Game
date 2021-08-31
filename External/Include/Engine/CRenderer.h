#pragma once
#include "CComponent.h"

#define COLOR_RGBA(r,g,b,a) (UINT)(((((BYTE)a << 24 ) | (BYTE)b << 16) | (BYTE)g << 8) | (BYTE)r)
#define COLOR_R_FROM_RGBA(r) (BYTE)r
#define COLOR_G_FROM_RGBA(g) (((BYTE)g >> 8) | (BYTE)0)
#define COLOR_B_FROM_RGBA(b) (((BYTE)b >> 16) | (BYTE)0)
#define COLOR_A_FROM_RGBA(a) (((BYTE)a >> 24) | (BYTE)0)

class CRenderer : public CComponent
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
	CLONE(CRenderer);
	CRenderer() = delete;
	CRenderer(E_ComponentType _eType);
	virtual ~CRenderer() override;
};