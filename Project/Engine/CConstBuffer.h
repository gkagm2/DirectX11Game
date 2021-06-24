#pragma once
#include "CObject.h"
class CConstBuffer : public CObject
{
private:
	ComPtr<ID3D11Buffer> m_pCB; // 상수 버퍼 Constant Buffer
	D3D11_BUFFER_DESC	 m_tDesc;
	E_ConstBuffer		 m_eType;

public:
	// buffer size, 몇번 레지스터?
	void Create(E_ConstBuffer _eType, UINT _iBufferSize);
	void SetData(void* _pSysMem, UINT _iSize = 0) const; // system memory -> buffer gpu
	void UpdateData(E_ShaderStage _eStage = E_ShaderStage::All) const ; // gpu data -> gpu register binding

private:
	CLONE_DISABLE(CConstBuffer);
public:
	CConstBuffer();
	virtual ~CConstBuffer() override;
};