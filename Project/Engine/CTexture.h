#pragma once
#include "CResource.h"
#include <DirectXTex\DirectXTex.h>

#ifdef _DEBUG || DEBUG
#pragma comment(lib, "DirectXTex\\DirectXTex_debug")
#else
#pragma comment(lib, "DirectXTex\\DirectXTex")
#endif

class CTexture : public CResource
{
private:
	ScratchImage m_Image;
	ComPtr<ID3D11Texture2D>             m_pTex2D;
	ComPtr<ID3D11ShaderResourceView>    m_pSRV;

private:
	virtual int Load(const tstring& _strFilePath);

public:
	void UpdateData(E_ShaderStage _eShaderStage, UINT _iRegisterNum);

public:
	CTexture();
	virtual ~CTexture() override;
};