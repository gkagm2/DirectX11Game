#pragma once
#include "CResource.h"
#include "CGraphicsShader.h"
#include "CTexture.h"

class CMaterial : public CResource
{
	SharedPtr<CGraphicsShader> m_pShader;
	TMaterialParam m_tParam;
	SharedPtr<CTexture> m_arrTexture[(UINT)E_ShaderParam::Texture_End - (UINT)E_ShaderParam::Texture_0];

	// TODO (Jang) :Test code
public:
	SharedPtr<CTexture> m_pTexture;

public:
	virtual int Load(const tstring& _strFilePath) override;
	virtual void UpdateData() override;

public:
	void SetShader(SharedPtr<CGraphicsShader> _pShader) { m_pShader = _pShader; }
	void SetData(E_ShaderParam _eParam, void* _pData);
	
public:
	CMaterial();
	virtual ~CMaterial() override;
};