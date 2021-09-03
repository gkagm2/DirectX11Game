#pragma once
#include "CImageUI.h"
class CButtonUI : public CImageUI
{
private:
	UINT m_iNormalColor;
	UINT m_iHighlightedColor;
	UINT m_iPressedColor;
	UINT m_iSelectedColor;
	UINT m_iDisableColor;
	float m_fFadeDuration;

public:
	virtual void FinalUpdate() override;
	virtual void UpdateData() override;
	virtual void Render() override;

public:
	void SetNormalColor(UINT _iColor) { m_iNormalColor = _iColor; }
	void SetHighlightedColor(UINT _iColor) { m_iHighlightedColor = _iColor; }
	void SetPressedColor(UINT _iColor) { m_iPressedColor = _iColor; }
	void SetSelectedColor(UINT _iColor) { m_iSelectedColor = _iColor; }
	void SetDisableColor(UINT _iColor) { m_iDisableColor = _iColor; }
	void SetFadeDuration(float _fDuration) { m_fFadeDuration = _fDuration; }

	UINT GetNormalColor() { return m_iNormalColor; }
	UINT GetHighlightedColor() { return m_iHighlightedColor; }
	UINT GetPressedColor() { return m_iPressedColor; }
	UINT GetSelectedColor() { return m_iSelectedColor; }
	UINT GetDisableColor() { return m_iDisableColor; }
	float GetFadeDuration() { return m_fFadeDuration; }

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CButtonUI);
	CButtonUI();
	virtual ~CButtonUI() override;
};