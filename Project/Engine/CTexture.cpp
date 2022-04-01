#include "pch.h"
#include "CTexture.h"
#include "CDevice.h"

CTexture::CTexture() :
	CResource(E_ResourceType::Texture),
	m_tDesc{}
{
}

CTexture::~CTexture()
{
}

int CTexture::Load(const tstring& _strFilePath)
{
	TCHAR strBuff[250] = {};
	
	// _tsplitpath_s(_strFilePath.c_str(), 0, 0, 0, 0, 0, 0, strBuff, 250);
	
	// GetExtension
	int iDotIdx = 0; 
	int size = (int)_strFilePath.size();
	for (int i = size - 1; i >= 0; --i) {
		if (_strFilePath[i] == _T('.')) {
			iDotIdx = i;
			break;
		}
	}

	int extLen = size - iDotIdx + 1;
	tstring strExt = _strFilePath.substr(iDotIdx, extLen);

	HRESULT hRet = S_OK;
	if (strExt == _T(".dds") || strExt == _T(".DDS")) {
		// .dds .DDS 포멧
		hRet = LoadFromDDSFile(_strFilePath.c_str(), DDS_FLAGS_NONE, nullptr, m_Image);
	}
	else if (strExt == _T(".tga") || strExt == _T(".TGA")) {
		// .tga .TGA 포멧
		hRet = LoadFromTGAFile(_strFilePath.c_str(), nullptr, m_Image);
	}
	else {
		// png, jpg, jpeg, bmp 포멧
		hRet = LoadFromWICFile(_strFilePath.c_str(), WIC_FLAGS_NONE, nullptr, m_Image);
	}

	if (FAILED(hRet)) {
		_tprintf_s(strBuff, _T("에러코드 : %d"), hRet);
		MessageBox(nullptr, strBuff, STR_MSG_FailedToLoadTexture, MB_OK);
		return hRet;
	}

	// 텍스쳐의 View 생성
	hRet = CreateShaderResourceView(
		DEVICE.Get(),
		m_Image.GetImages(),
		m_Image.GetImageCount(),
		m_Image.GetMetadata(),
		m_pSRV.GetAddressOf());

	if (FAILED(hRet)) {
		_tprintf_s(strBuff, _T("에러코드 : %d"), hRet);
		MessageBox(nullptr, strBuff, STR_MSG_FailedToCreateShaderResourceView, MB_OK);
		return hRet;
	}

	// 리소스 가져오기
	m_pSRV->GetResource((ID3D11Resource**)m_pTex2D.GetAddressOf());

	// 텍스쳐 정보 저장
	m_pTex2D->GetDesc(&m_tDesc);

	return hRet;
}


void CTexture::Create(UINT _iWidth, UINT _iHeight, DXGI_FORMAT _eFormat, UINT _iBindFlag)
{
	m_tDesc.Width = _iWidth;
	m_tDesc.Height = _iHeight;
	m_tDesc.MipLevels = 1; // 원본 하나만 지정
	m_tDesc.ArraySize = 1;
	m_tDesc.Format = _eFormat; // 픽셀포멧

	m_tDesc.SampleDesc.Count = 1;
	m_tDesc.SampleDesc.Quality = 0;

	m_tDesc.BindFlags = _iBindFlag;
	m_tDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tDesc.CPUAccessFlags = 0;

	HRESULT hr = DEVICE->CreateTexture2D(&m_tDesc, nullptr, m_pTex2D.GetAddressOf());
	if (FAILED(hr)) assert(nullptr);

	_CreateTexture2D_InnerFunc();
}

void CTexture::Create(ComPtr<ID3D11Texture2D> _pTexture2D)
{
	m_pTex2D = _pTexture2D;
	m_pTex2D->GetDesc(&m_tDesc); // 생성된 텍스쳐2D의 정보를 받아옴.
	_CreateTexture2D_InnerFunc();
}

void CTexture::_CreateTexture2D_InnerFunc()
{
	HRESULT hr = 0;

	// 2. 생성한 DepthStencil Texture로 DepthStencilView를 생성한다.
	if (D3D11_BIND_DEPTH_STENCIL & m_tDesc.BindFlags) {
		//D3D11_DEPTH_STENCIL_VIEW_DESC tDSVDesc = {};
		hr = DEVICE->CreateDepthStencilView(m_pTex2D.Get(), 0, m_pDSV.GetAddressOf());
		if (FAILED(hr)) assert(nullptr);
	}
	else {
		if (D3D11_BIND_RENDER_TARGET & m_tDesc.BindFlags) {
			//D3D11_RENDER_TARGET_VIEW_DESC tRTVDesc = {};
			hr = DEVICE->CreateRenderTargetView(m_pTex2D.Get(), 0, m_pRTV.GetAddressOf());
			if (FAILED(hr)) assert(nullptr);
		}

		if (D3D11_BIND_SHADER_RESOURCE & m_tDesc.BindFlags) {
			/*D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
			tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			tSRVDesc.Texture2D.MipLevels = 1;
			tSRVDesc.Texture2D.MostDetailedMip = 0;
			hr = DEVICE->CreateShaderResourceView(m_pTex2D.Get(), &tSRVDesc, m_pSRV.GetAddressOf());*/
			hr = DEVICE->CreateShaderResourceView(m_pTex2D.Get(), 0, m_pSRV.GetAddressOf());

			if (FAILED(hr)) assert(nullptr);
		}

		if (D3D11_BIND_UNORDERED_ACCESS & m_tDesc.BindFlags) {
			/*D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVDesc = {};
			tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
			hr = DEVICE->CreateUnorderedAccessView(m_pTex2D.Get(), &tUAVDesc, m_pUAV.GetAddressOf());
			D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVDesc = {};
			tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
			*/
			hr = DEVICE->CreateUnorderedAccessView(m_pTex2D.Get(), 0, m_pUAV.GetAddressOf());
			if (FAILED(hr)) assert(nullptr);
		}
	}
}

void CTexture::UpdateData(E_ShaderStage _eShaderStage, UINT _iRegisterNum)
{
	if((UINT)E_ShaderStage::Vertex & (UINT)_eShaderStage)
		CONTEXT->VSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());
	if ((UINT)E_ShaderStage::Hull & (UINT)_eShaderStage)
		CONTEXT->HSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());
	if ((UINT)E_ShaderStage::Domain & (UINT)_eShaderStage)
		CONTEXT->DSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());
	if ((UINT)E_ShaderStage::Geometry & (UINT)_eShaderStage)
		CONTEXT->GSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());
	if ((UINT)E_ShaderStage::Pixel & (UINT)_eShaderStage)
		CONTEXT->PSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());
	if ((UINT)E_ShaderStage::Compute & (UINT)_eShaderStage)
		CONTEXT->CSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());
}

void CTexture::UpdateRWData(UINT _iUAVRegisterNum)
{
	ID3D11UnorderedAccessView* pUAV = m_pUAV.Get();
	UINT iUAVIntialCounts = -1;
	CONTEXT->CSSetUnorderedAccessViews(_iUAVRegisterNum, 1, &pUAV, &iUAVIntialCounts);
}

void CTexture::Clear(UINT _iRegisterNum)
{
	ID3D11ShaderResourceView* pSRV = nullptr;
	CONTEXT->VSSetShaderResources(_iRegisterNum, 1, &pSRV);
	CONTEXT->HSSetShaderResources(_iRegisterNum, 1, &pSRV);
	CONTEXT->DSSetShaderResources(_iRegisterNum, 1, &pSRV);
	CONTEXT->GSSetShaderResources(_iRegisterNum, 1, &pSRV);
	CONTEXT->PSSetShaderResources(_iRegisterNum, 1, &pSRV);
	CONTEXT->CSSetShaderResources(_iRegisterNum, 1, &pSRV);
}

void CTexture::ClearRW(UINT _iUAVRegisterNum)
{
	ID3D11UnorderedAccessView* pUAV = nullptr;
	UINT iUAVInitialCounts = -1;
	CONTEXT->CSSetUnorderedAccessViews(_iUAVRegisterNum, 1, &pUAV, &iUAVInitialCounts);
}