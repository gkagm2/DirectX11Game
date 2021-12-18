#pragma once
#include "CObject.h"

enum class E_StructuredBufferType {
	ReadOnly, // 파이프라인에서 읽기만 가능
	Read_Write, // 시스템 메모리에서 중간에 데이터세팅을 할 수 없음. 컴퓨트 쉐이더에서 RWStrctured 사용 가능
};

class CStructuredBuffer : public CObject
{
private:
	ComPtr<ID3D11Buffer> m_SB;
	D3D11_BUFFER_DESC m_desc;

	ComPtr<ID3D11ShaderResourceView> m_SRV;
	ComPtr<ID3D11UnorderedAccessView> m_UAV;

	UINT m_iElementSize;
	UINT m_iElementCount;
	E_StructuredBufferType m_eType;

	ComPtr<ID3D11Buffer> m_SB_CPU_Read;
	ComPtr<ID3D11Buffer >m_SB_CPU_Write;

	D3D11_BUFFER_DESC m_desc_cpu_read;
	D3D11_BUFFER_DESC m_desc_cpu_write;

	bool m_bCpuAccess;

	// 가장 최근에 사용된 값
	E_ShaderStage m_eRecentPipelineStage; // 최근에 바인딩 된 파이프라인 시점
	UINT m_iRecentRegisterNum; // 최근에 바인딩 된 t 레지스터 번호
	UINT m_iRecentRegisterNumRW; // 최근에 바인딩 된 u 레지스터 번호
public:
	HRESULT Create(E_StructuredBufferType _eType, UINT _iElementSize, UINT _iElementCount, bool _bUseSysIO, void* _pInitial = nullptr);

	void SetData(void* _pSysMem, UINT _iSize) const; // system memory -> buffer gpu
	void GetData(void* _pDest, UINT _iSize);


	// gpu data -> gpu register binding
	// Shader Resource Binding (t register)
	void UpdateData(UINT _iRegisterNum, E_ShaderStage _eStage = E_ShaderStage::All); 
	// Unordered Access Binding (u register)
	void UpdateDataCS(UINT _iRegisterNum); // Compute Shader

	UINT GetElementCount() { return m_iElementCount; }
	UINT GetElementSize() { return m_iElementSize; }

	//Shader Resource Clear
	void Clear(E_ShaderStage _eStage = E_ShaderStage::All);

	// Unordered Access Clear
	void ClearRW();	

	void Release();

private:
	CLONE_DISABLE(CStructuredBuffer);

public:
	CStructuredBuffer();
	virtual ~CStructuredBuffer() override;
};