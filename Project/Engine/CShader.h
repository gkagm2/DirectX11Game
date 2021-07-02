#pragma once
#include "CResource.h"

class CShader : public CResource
{
protected:
	ComPtr<ID3DBlob>				m_pErrBlob; // 에러 메세지 저장 용도
private:
	virtual void UpdateData() = 0;
	virtual int Load(const tstring& _strFilePath) override { return S_OK; }
	
private:
	CLONE_DISABLE(CShader);

public:
	CShader();
	virtual ~CShader() override;
};