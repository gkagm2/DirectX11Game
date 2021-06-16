#pragma once
#include "CResource.h"
#include <DirectXTex\DirectXTex.h>

#if (defined _DEBUG) || (defined DEBUG)
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
	D3D11_TEXTURE2D_DESC				m_tDesc;

private:
	virtual int Load(const tstring& _strFilePath);

public:
	void UpdateData(E_ShaderStage _eShaderStage, UINT _iRegisterNum);

public:
	Vector2 GetDimension() { return std::move(Vector2((float)m_tDesc.Width, (float)m_tDesc.Height)); }

public:
	static void Clear(UINT _iRegisterNum);

public:
	CTexture();
	virtual ~CTexture() override;
};