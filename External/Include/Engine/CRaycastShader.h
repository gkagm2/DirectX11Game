#pragma once
#include "CComputeShader.h"
class CRaycastShader : public CComputeShader
{
private:
    // 지형의 높이 정보
    SharedPtr<CTexture> m_pHeightMap;

    // 지형 면 개수
    UINT m_iXFace;
    UINT m_iZFace;

    // 카메라 Ray 정보
    TRay m_tRay;

    // 교점위치 출력 버퍼
    CStructuredBuffer* m_pOutputBuffer;

public:
    void SetFaceCount(UINT _x, UINT _z) { m_iXFace = _x; m_iZFace = _z; }
    void SetCameraRay(const TRay& _ray) { m_tRay = _ray; }
    void SetOuputBuffer(CStructuredBuffer* _pOutputBuffer) { m_pOutputBuffer = _pOutputBuffer; }
    void SetHeightMap(SharedPtr<CTexture> _ptex) { m_pHeightMap = _ptex; }

public:
    virtual void Excute();
    virtual void UpdateData();
    virtual void Clear();

public:
	CRaycastShader();
	virtual ~CRaycastShader() override;
};