#pragma once
#include "CShader.h"
class CComputeShader : public CShader
{
private:
	virtual int Load(const tstring& _strFilePath) override { return S_OK; }
public:
	CComputeShader();
	virtual ~CComputeShader() override;
};