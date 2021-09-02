#pragma once
#include "CImageUI.h"
class CButtonUI : public CImageUI
{
private:
	Vector4 m_fNormalColor;
	Vector4 m_fHighlightedColor;
	Vector4 m_fPressedColor;
	Vector4 m_fSelectedColor;
	Vector4 m_fDisableColor;
	float m_fFadeDuration;

public:
	virtual void FinalUpdate() override;
	virtual void UpdateData() override;
	virtual void Render() override;

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CButtonUI);
	CButtonUI();
	virtual ~CButtonUI() override;
};