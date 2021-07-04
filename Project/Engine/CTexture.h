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
	ComPtr<ID3D11UnorderedAccessView>	m_pUAV;
	ComPtr<ID3D11RenderTargetView>		m_pRTV;

	ComPtr<ID3D11DepthStencilView>		m_pDSV;

	D3D11_TEXTURE2D_DESC				m_tDesc;



private:
	virtual int Load(const tstring& _strFilePath);

	void Create(UINT _iWidth, UINT _iHeight, DXGI_FORMAT _eFormat, UINT _iBindFlag);
	void Create(ComPtr<ID3D11Texture2D> _pTexture2D);
	void _CreateTexture2D_InnerFunc(); // Create함수 내부에서 사용.

public:
	void UpdateData(E_ShaderStage _eShaderStage, UINT _iRegisterNum);
	void UpdateRWData(UINT _iUAVRegisterNum);

public:
	Vector2 GetDimension() { return std::move(Vector2((float)m_tDesc.Width, (float)m_tDesc.Height)); }

public:
	static void Clear(UINT _iRegisterNum);
	static void ClearRW(UINT _iUAVRegisterNum);

	ComPtr<ID3D11ShaderResourceView> GetSRV() { return m_pSRV; }
	ComPtr<ID3D11UnorderedAccessView>GetUAV() { return m_pUAV; }
	ComPtr<ID3D11RenderTargetView>	GetRTV() { return m_pRTV; }

	ComPtr<ID3D11DepthStencilView>	GetDSV() { return m_pDSV; }

private:
	CLONE_DISABLE(CTexture);

public:
	CTexture();
	virtual ~CTexture() override;

	friend class CResourceManager;
};