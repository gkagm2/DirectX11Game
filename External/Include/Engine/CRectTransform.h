#pragma once
#include "CTransform.h"
class CRectTransform : public CTransform
{
private:
	float m_fWidth;
	float m_fHeight;
	Matrix m_matLocal_noParentScale;
	Matrix m_matWorld_noParentScale; // 부모 자식간 scale을 고려하지 않은 행렬

public:
	virtual void FinalUpdate() override;
	virtual void UpdateData() override;

public:
	void SetWidth(float _fWidth) { m_fWidth = _fWidth; }
	void SetHeight(float _fHeight) { m_fHeight = _fHeight; }
	float GetWidth() { return m_fWidth; }
	float GetHeight() { return m_fHeight; }

public:
	const Matrix& GetWorldMatrix_NoParentScale() { return m_matWorld_noParentScale; }
	const Matrix& GetLocalMatrix_NoParentScale() { return m_matLocal_noParentScale; }

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;
public:
	CLONE(CRectTransform);
	CRectTransform();
	virtual ~CRectTransform() override;
};