#pragma once
#include "CObject.h"

enum class E_StructuredBufferType {
	ReadOnly, // ���������ο��� �б⸸ ����
	Read_Write, // �ý��� �޸𸮿��� �߰��� �����ͼ����� �� �� ����. ��ǻƮ ���̴����� RWStrctured ��� ����
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

	// ���� �ֱٿ� ���� ��
	E_ShaderStage m_eRecentPipelineStage; // �ֱٿ� ���ε� �� ���������� ����
	UINT m_iRecentRegisterNum; // �ֱٿ� ���ε� �� t �������� ��ȣ
	UINT m_iRecentRegisterNumRW; // �ֱٿ� ���ε� �� u �������� ��ȣ
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