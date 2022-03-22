#pragma once
#include "CComputeShader.h"
class CHeightMapShader : public CComputeShader
{
private:
	SharedPtr<CTexture> m_pHeightMapTex;
	SharedPtr<CTexture> m_pBrushTex;
	Vector2 m_vScale;
	int m_iBrushIdx;
	CStructuredBuffer* m_pInputBuffer; // Ray 충돌 위치

public:
	virtual void UpdateData() override;
	virtual void Excute() override;
	virtual void Clear() override;

public:
	void SetHeigtMapTexture(SharedPtr<CTexture> _pHeightMapTex) { m_pHeightMapTex = _pHeightMapTex; }
	void SetBrushTexture(SharedPtr<CTexture> _pBrushTex) { m_pBrushTex = _pBrushTex; }
	void SetBrushScale(const Vector2& _vScale) { m_vScale = _vScale; }
	void SetBrushIdx(int _iIdx) { m_iBrushIdx = _iIdx; }
	void SetInputBuffer(CStructuredBuffer* _pInputBuffer) { m_pInputBuffer = _pInputBuffer; }
	SharedPtr<CTexture> GetHeightMapTexture() { return m_pHeightMapTex; }
	SharedPtr<CTexture> GetBrushTexture() { return m_pBrushTex; }
	const Vector2& GetBrushScale() { return m_vScale; }
	int GetBrushIdx() { return m_iBrushIdx; }

public:
	CHeightMapShader();
	virtual ~CHeightMapShader() override;
};