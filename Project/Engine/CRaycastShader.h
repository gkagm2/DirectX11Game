#pragma once
#include "CComputeShader.h"
class CRaycastShader : public CComputeShader
{
private:
    // ������ ���� ����
    SharedPtr<CTexture> m_pHeightMap;

    // ���� �� ����
    UINT m_iXFace;
    UINT m_iZFace;

    // ī�޶� Ray ����
    TRay m_tRay;

    // ������ġ ��� ����
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