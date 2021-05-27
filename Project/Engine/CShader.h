#pragma once
#include "CResource.h"

class CShader : public CResource
{
public:
	virtual void Load(const tstring& _strFilePath) override {};
public:
	CShader();
	virtual ~CShader() override;
};