#pragma once
#include "CResource.h"

class CShader : public CResource
{
private:
	virtual int Load(const tstring& _strFilePath) override { return S_OK; }
public:
	CShader();
	virtual ~CShader() override;
};