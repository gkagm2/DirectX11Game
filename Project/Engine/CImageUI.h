#pragma once
#include "CUI.h"
class CImageUI : public CUI
{
private:
	SharedPtr<CTexture> m_pTexture;
	UINT m_iColor;

public:
	virtual void FinalUpdate() override;
	virtual void UpdateData() override;
	virtual void Render() override; // FIXED (Jang) :필요 없을수도 있음.

public:
	void SetImageTex(SharedPtr<CTexture> _pTexture) { m_pTexture = _pTexture; }
	SharedPtr<CTexture> GetImageTex() { return m_pTexture; }
	void SetColor(UINT _iColor) { m_iColor = _iColor; }
	UINT GetColor() { return m_iColor; }

public:
	CLONE(CImageUI);
	CImageUI();
	virtual ~CImageUI() override;
};