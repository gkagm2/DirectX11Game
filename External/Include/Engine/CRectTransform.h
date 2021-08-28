#pragma once
#include "CTransform.h"
class CRectTransform : public CTransform
{
private:
	float m_fWidth;
	float m_fHeight;
public:
	virtual void FinalUpdate() override;
	virtual void UpdateData() override;

public:
	void SetWidth(float _fWidth) { m_fWidth = _fWidth; }
	void SetHeight(float _fHeight) { m_fHeight = _fHeight; }
	float GetWidth() { return m_fWidth; }
	float GetHeight() { return m_fHeight; }

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;
public:
	CLONE(CRectTransform);
	CRectTransform();
	virtual ~CRectTransform() override;
};