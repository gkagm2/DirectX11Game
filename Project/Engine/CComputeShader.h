#pragma once
#include "CShader.h"
class CComputeShader : public CShader
{
public:
	virtual void Load(const tstring& _strFilePath) override {};
public:
	CComputeShader();
	virtual ~CComputeShader() override;
};