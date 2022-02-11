#pragma once
#include "CUI.h"
#include "CFontManager.h"

enum class E_TextAlign {
	LeftTop,
	MiddleTop,
	RightTop,
	Left,
	Center,
	Right,
	LeftBottom,
	MiddleBottom,
	RightBottom
};

class CTextUI : public CUI
{
private:
	tstring m_strText;
	float m_fFontSize;
	Vector2 m_vScreenPos;
	UINT m_iColor; // Text color, 0xAaBbGgRr

	E_TextAlign m_eTextAlignment;
	tstring m_strTextAlignment;
	TFont m_tFontType;

private:
	FW1_TEXT_FLAG m_eFlag;

public:
	virtual void FinalUpdate() override;
	virtual void UpdateData() override;
	virtual void Render() override;

public:

	const TFont& GetFontType() { return m_tFontType; }
	void SetFontType(int iIdx);
	void SetText(const tstring& _strText) { m_strText = _strText; }
	const tstring& GetText() { return m_strText; }

	void SetColor(UINT _r, UINT _g, UINT _b, UINT _a) { m_iColor = FONT_RGBA(_r, _g, _b, _a); }
	void SetColor(UINT _iColor) { m_iColor = _iColor; }
	UINT GetColor() { return m_iColor; }
	float GetFontSize() { return m_fFontSize; }
	void SetFontSize(float _fSize) { m_fFontSize = _fSize; }

	void SetAlign(E_TextAlign _eTextAlignment);
	E_TextAlign GetAlign() { return m_eTextAlignment; }
	const tstring& GetAlignText() { return m_strTextAlignment; }

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CTextUI);
	CTextUI();
	virtual ~CTextUI() override;
};