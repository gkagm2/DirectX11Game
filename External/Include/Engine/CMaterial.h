#pragma once
#include "CResource.h"
#include "CGraphicsShader.h"
#include "CTexture.h"

class CMaterial : public CResource
{
private:
	SharedPtr<CGraphicsShader> m_pShader;

	TMaterialParam m_tParam;
	SharedPtr<CTexture> m_arrTexture[(UINT)E_ShaderParam::Texture_End - (UINT)E_ShaderParam::Texture_0];

private:
	virtual int Load(const tstring& _strFilePath) override;

public:
	virtual void UpdateData() override;

public:
	void SetShader(SharedPtr<CGraphicsShader>& _pShader) { m_pShader = _pShader; }
	void SetData(E_ShaderParam _eParam, void* _pData);
	
public:
	virtual CMaterial* Clone() override;
	CMaterial();
	virtual ~CMaterial() override;
};