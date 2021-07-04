#pragma once
#include "CObject.h"
class CStructuredBuffer : public CObject
{
private:
	ComPtr<ID3D11Buffer> m_pSB; // 구조체 버퍼 SB : Structed Buffer

	ComPtr<ID3D11ShaderResourceView> m_pSRV;  // 쉐이더 리소스 뷰
	ComPtr<ID3D11UnorderedAccessView> m_pUAV;

	D3D11_BUFFER_DESC m_tDesc;

	UINT m_iElementSize;
	UINT m_iElementCount;

	UINT m_iRecentRegisterNum;

public:
	void Create(UINT _iElementSize, UINT _iElementCount, void* _pSysData = nullptr);
	void CreateRW(UINT _iElementSize, UINT _iElementCount, void* _pSysData = nullptr);
	void SetData(void* _pSysMem, UINT _iElementCount) const; // system memory -> buffer gpu
	void UpdateData(UINT _iRegisterNum, E_ShaderStage _eStage = E_ShaderStage::All); // gpu data -> gpu register binding
	void UpdateDataRW(UINT _iRegisterNum);

	UINT GetElementCount() { return m_iElementCount; }
	UINT GetElementSize() { return m_iElementSize; }

	void Clear(E_ShaderStage _eStage = E_ShaderStage::All);
	void ClearRW();	

	void Release();

private:
	CLONE_DISABLE(CStructuredBuffer);

public:
	CStructuredBuffer();
	virtual ~CStructuredBuffer() override;
};