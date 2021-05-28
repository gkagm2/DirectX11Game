#pragma once
#include "CEntity.h"
class CConstBuffer : public CEntity
{
private:
	ComPtr<ID3D11Buffer> m_pCB; // ��� ���� Constant Buffer
	D3D11_BUFFER_DESC	 m_tDesc;
	E_ConstBuffer		 m_eType;

public:
	// buffer size, ��� ��������?
	void Create(E_ConstBuffer _eType, UINT _iBufferSize);
	void SetData(void* _pSysMem, UINT _iSize = 0) const; // system memory -> buffer gpu
	void UpdateData(E_ShaderStage _eStage = E_ShaderStage::all) const ; // gpu data -> gpu register binding

public:
	CConstBuffer();
	virtual ~CConstBuffer() override;
};