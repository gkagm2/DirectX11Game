#pragma once
#include "CUI.h"
class CImageUI : public CUI
{
private:
	SharedPtr<CMesh> m_pMesh;
	SharedPtr<CMaterial> m_pMtrl;
	SharedPtr<CTexture> m_pTexture;
	UINT m_iColor;

public:
	virtual void FinalUpdate() override;
	virtual void Render() override; // FIXED (Jang) :필요 없을수도 있음.

public:
	void SetImageTex(SharedPtr<CTexture> _pTexture);
	SharedPtr<CTexture> GetImageTex();
	void SetColor(UINT _iColor) { m_iColor = _iColor; }
	UINT GetColor() { return m_iColor; }

	SharedPtr<CMesh> GetMesh() { return m_pMesh; }
	SharedPtr<CMaterial> GetCloneMaterial();
	SharedPtr<CMaterial> GetSharedMaterial() { return m_pMtrl; }

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CImageUI);
	CImageUI();
	CImageUI(E_ComponentType _eType);
	virtual ~CImageUI() override;
};