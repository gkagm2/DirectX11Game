#pragma once
#include "CImageUI.h"

enum class E_ButtonState {
	Normal,
	Highlighted,
	Pressed,
	Selected,
	Disable,
	End
};

class CButtonUI : public CImageUI
{
private:
	UINT m_iNormalColor;
	UINT m_iHighlightedColor;
	UINT m_iPressedColor;
	UINT m_iSelectedColor;
	UINT m_iDisableColor;
	float m_fFadeDuration;

	E_ButtonState m_eButtonState;

public:
	virtual void UpdateData() override {}
	virtual void FinalUpdate() override;

public:
	void SetNormalColor(UINT _iColor);
	void SetHighlightedColor(UINT _iColor);
	void SetPressedColor(UINT _iColor);
	void SetSelectedColor(UINT _iColor);
	void SetDisableColor(UINT _iColor);
	void SetFadeDuration(float _fDuration) { m_fFadeDuration = _fDuration; }

	UINT GetNormalColor() { return GetColor(); }
	UINT GetHighlightedColor() { return m_iHighlightedColor; }
	UINT GetPressedColor() { return m_iPressedColor; }
	UINT GetSelectedColor() { return m_iSelectedColor; }
	UINT GetDisableColor() { return m_iDisableColor; }
	float GetFadeDuration() { return m_fFadeDuration; }

	// TODO (Jang) : UI의 Click event가 올 때 상태를 바꾸자.
	void ChangeButtonState(E_ButtonState _eState) { m_eButtonState = _eState; }

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CButtonUI);
	CButtonUI();
	CButtonUI(const CButtonUI& _origin);
	virtual ~CButtonUI() override;
};