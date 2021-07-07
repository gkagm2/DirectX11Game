#pragma once
#include "CObject.h"

enum class E_StructuredBufferType {
	ReadOnly, // 파이프라인에서 읽기만 가능
	Read_Write, // 시스템 메모리에서 중간에 데이터세팅을 할 수 없음. 컴퓨트 쉐이더에서 RWStrctured 사용 가능
	Dual // CPU Memory와 GPU Memory와 Compute Shader사이에 데이터를 주고 받을 수 있음
};

class CStructuredBuffer : public CObject
{
private:
	ComPtr<ID3D11Buffer> m_pSB_R; // 구조체 버퍼 SB : Structed Buffer, R: read
	ComPtr<ID3D11Buffer >m_pSB_RW; // RW : Read Write

	ComPtr<ID3D11ShaderResourceView> m_pSRV;  // 쉐이더 리소스 뷰
	ComPtr<ID3D11UnorderedAccessView> m_pUAV;

	D3D11_BUFFER_DESC m_tDesc;

	UINT m_iElementSize;
	UINT m_iElementCount;

	UINT m_iRecentRegisterNum;

	E_StructuredBufferType m_eType;

public:
	void Create(E_StructuredBufferType _eType, UINT _iElementSize, UINT _iElementCount, void* _pSysData = nullptr);

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