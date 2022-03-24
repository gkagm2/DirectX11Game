#pragma once
#include "CComputeShader.h"
class CWeightMapShader : public CComputeShader
{
private:
	CStructuredBuffer* m_pWeightMapBuff;
	CStructuredBuffer* m_pRaycastInfoBuff;

	SharedPtr<CTexture> m_pBrushArrTex;
	Vector2 m_vBrushScale;
	int m_iBrushIdx;
	int m_iWeightIdx; // 가중치 수정할 위치

	int m_iWidth;
	int m_iHeight;

public:
	virtual void UpdateData() override;
	virtual void Excute() override;
	virtual void Clear() override;

public:
	void SetWeightMap(CStructuredBuffer* _pWeightMap, UINT _iWidth, UINT _iHeight) {
		m_pWeightMapBuff = _pWeightMap;
		m_iWidth = (int)_iWidth;
		m_iHeight = (int)_iHeight;
	}
	void SetRaycasatInputBuffer(CStructuredBuffer* _pRaycastInfo) { m_pRaycastInfoBuff = _pRaycastInfo; }
	void SetBrushArrTex(SharedPtr<CTexture>_pBrushTex) { m_pBrushArrTex = _pBrushTex;	}
	void SetBrushScale(const Vector2& _vScale) { m_vBrushScale = _vScale; }
	void SetBrushIdx(int _iIdx) { m_iBrushIdx = _iIdx; }
	void SetWeightIdx(int _iIdx) { m_iWeightIdx = _iIdx; }

	SharedPtr<CTexture> GetBrushArrTex() { return m_pBrushArrTex; }
	const Vector2& GetBrushScale() { return m_vBrushScale; }
	int GetBrushIdx() { return m_iBrushIdx; }
	int GetWeightIdx() { return m_iWeightIdx; }

public:
	CWeightMapShader();
	virtual ~CWeightMapShader() override;
};