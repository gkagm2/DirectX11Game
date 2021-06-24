#pragma once
#include "CObject.h"
class CStructuredBuffer : public CObject
{
private:
	ComPtr<ID3D11Buffer> m_pSB; // 구조체 버퍼 SB : Structed Buffer
	ComPtr<ID3D11ShaderResourceView> m_pSRV; // 쉐이더 리소스 뷰
	D3D11_BUFFER_DESC m_tDesc;

	UINT m_iElementSize;
	UINT m_iElementCount;

public:
	void Create(UINT _iElementSize, UINT _iElementCount);
	void SetData(void* _pSysMem, UINT _iSize = 0) const; // system memory -> buffer gpu
	void UpdateData(E_ShaderStage _eStage = E_ShaderStage::All) const; // gpu data -> gpu register binding

private:
	CLONE_DISABLE(CStructuredBuffer);

public:
	CStructuredBuffer();
	virtual ~CStructuredBuffer() override;
};