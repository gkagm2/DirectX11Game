#pragma once
#include "CComputeShader.h"
class CHeightMapShader : public CComputeShader
{
public:
	virtual void UpdateData() override;
	virtual void Clear() override;
	virtual void Excute() override;

public:
	CHeightMapShader();
	virtual ~CHeightMapShader() override;
};