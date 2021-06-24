#pragma once
#include "CResource.h"

class CShader : public CResource
{
private:
	virtual void UpdateData() = 0;
	virtual int Load(const tstring& _strFilePath) override { return S_OK; }
	
private:
	CLONE_DISABLE(CShader);

public:
	CShader();
	virtual ~CShader() override;
};