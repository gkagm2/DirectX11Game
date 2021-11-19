#pragma once
#include "CObject.h"

enum class E_StructuredBufferType {
	ReadOnly, // ���������ο��� �б⸸ ����
	Read_Write, // �ý��� �޸𸮿��� �߰��� �����ͼ����� �� �� ����. ��ǻƮ ���̴����� RWStrctured ��� ����
	Dual // CPU Memory�� GPU Memory�� Compute Shader���̿� �����͸� �ְ� ���� �� ����
};

class CStructuredBuffer : public CObject
{
private:
	ComPtr<ID3D11Buffer> m_pSB_R; // ����ü ���� SB : Structed Buffer, R: read
	ComPtr<ID3D11Buffer >m_pSB_RW; // RW : Read Write

	ComPtr<ID3D11ShaderResourceView> m_pSRV;  // ���̴� ���ҽ� ��
	ComPtr<ID3D11UnorderedAccessView> m_pUAV;

	D3D11_BUFFER_DESC m_tDesc;

	UINT m_iElementSize;
	UINT m_iElementCount;

	UINT m_iRecentRegisterNum;

	E_StructuredBufferType m_eType;

public:
	void Create(E_StructuredBufferType _eType, UINT _iElementSize, UINT _iElementCount, void* _pSysData = nullptr);

	void SetData(void* _pSysMem, UINT _iElementCount) const; // system memory -> buffer gpu
	void GetData(void* _pDest, UINT _iElementCount);


	// gpu data -> gpu register binding
	// Shader Resource Binding (t register)
	void UpdateData(UINT _iRegisterNum, E_ShaderStage _eStage = E_ShaderStage::All); 
	// Unordered Access Binding (u register)
	void UpdateDataRW(UINT _iRegisterNum);

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