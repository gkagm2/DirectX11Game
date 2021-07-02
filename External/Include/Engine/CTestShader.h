#pragma once
#include "CComputeShader.h"
class CTestShader : public CComputeShader
{
private:
	Vector4 m_vSetColor;
	SharedPtr<CTexture> m_pTexture;
public:
	virtual void UpdateData() override;
	virtual void Clear() override;
	virtual void Excute() override;

public:
	void SetTexture(SharedPtr<CTexture> _pTexture) { m_pTexture = _pTexture; }
	void SetClearColor(const Vector4& _vColor) { m_vSetColor = _vColor; }

public:
	CTestShader();
	virtual ~CTestShader() override;
};