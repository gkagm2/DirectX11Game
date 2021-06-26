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
	void SetData(void* _pSysMem, UINT _iElementCount) const; // system memory -> buffer gpu
	void UpdateData(UINT _iRegisterNum, E_ShaderStage _eStage = E_ShaderStage::All) const; // gpu data -> gpu register binding

	UINT GetElementCount() { return m_iElementCount; }
	UINT GetElementSize() { return m_iElementSize; }

	void Release();

private:
	CLONE_DISABLE(CStructuredBuffer);

public:
	CStructuredBuffer();
	virtual ~CStructuredBuffer() override;
};