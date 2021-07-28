#pragma once
#include "CResource.h"

class CShader : public CResource
{
protected:
	ComPtr<ID3DBlob>				m_pErrBlob; // ���� �޼��� ���� �뵵
private:
	virtual void UpdateData() = 0;
	virtual int Load(const tstring& _strFilePath) override { return S_OK; }
	
private:
	CLONE_DISABLE(CShader);

public:
	CShader(E_ResourceType _eType);
	virtual ~CShader() override;
};