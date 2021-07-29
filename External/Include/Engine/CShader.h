#pragma once
#include "CResource.h"

struct TShaderParam {
	E_ShaderParam eType;
	tstring strName;
};

class CShader : public CResource
{
protected:
	ComPtr<ID3DBlob>	 m_pErrBlob; // ���� �޼��� ���� �뵵
	vector<TShaderParam> m_vecParam; // ���̴��� ����ϴ� Register ���

private:
	virtual void UpdateData() = 0;
	virtual int Load(const tstring& _strFilePath) override { return S_OK; }

public:
	void AddShaderParam(const TShaderParam& _tShaderParam) { m_vecParam.push_back(_tShaderParam); }
	const vector<TShaderParam>& GetShaderParams() { return m_vecParam; }

private:
	CLONE_DISABLE(CShader);

public:
	CShader(E_ResourceType _eType);
	virtual ~CShader() override;
};