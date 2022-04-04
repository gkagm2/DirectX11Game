#include "pch.h"
#include "CTexture.h"
#include "CDevice.h"
#include "CPathManager.h"

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
	return Load(_strFilePath, 1);
}

int CTexture::Load(const tstring& _strFilePath, int _iMipLevel)
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
		hRet = LoadFromDDSFile(_strFilePath.c_str(), DDS_FLAGS_FORCE_RGB, nullptr, m_Image);
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

	// Texture2D 생성
	m_tDesc.Format = m_Image.GetMetadata().format;
	
	if (m_Image.GetMetadata().IsCubemap()) {
		m_tDesc.MipLevels = 1;	// 0 ==> 가능한 모든 밉맵이 저장 될 수 있는 공간이 만들어짐
		m_tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		m_tDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	}
	else {
		m_tDesc.MipLevels = _iMipLevel;// MAX_MIP;	// 0 ==> 가능한 모든 밉맵이 저장 될 수 있는 공간이 만들어짐	
		m_tDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		m_tDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	}

	m_tDesc.ArraySize = m_Image.GetMetadata().arraySize;

	m_tDesc.SampleDesc.Count = 1;
	m_tDesc.SampleDesc.Quality = 0;

	m_tDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tDesc.CPUAccessFlags = 0;

	m_tDesc.Width = m_Image.GetMetadata().width;
	m_tDesc.Height = m_Image.GetMetadata().height;

	HRESULT hr = DEVICE->CreateTexture2D(&m_tDesc, nullptr, m_pTex2D.GetAddressOf());
	if (FAILED(hr)) assert(nullptr);

	// 원본데이터(밉맵 레벨 0)를 각 칸에 옮긴다.	
	for (int i = 0; i < m_tDesc.ArraySize; ++i) {
		// GPU 에 데이터 옮기기(밉맵 포함)
		UINT iSubresIdx = D3D11CalcSubresource(0, i, m_tDesc.MipLevels);

		CONTEXT->UpdateSubresource(m_pTex2D.Get(), iSubresIdx, nullptr
			, m_Image.GetImage(0, i, 0)->pixels
			, m_Image.GetImage(0, i, 0)->rowPitch
			, m_Image.GetImage(0, i, 0)->slicePitch);
	}

	// SRV 생성
	D3D11_SHADER_RESOURCE_VIEW_DESC viewdesc = {};
	viewdesc.Format = m_tDesc.Format;

	if (m_Image.GetMetadata().IsCubemap())
		viewdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	else if (1 < m_tDesc.ArraySize)
		viewdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	else
		viewdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	if (1 < m_tDesc.ArraySize) {
		viewdesc.Texture2DArray.ArraySize = m_tDesc.ArraySize;
		viewdesc.Texture2DArray.MipLevels = m_tDesc.MipLevels;
		viewdesc.Texture2DArray.MostDetailedMip = 0;
	}
	else {
		viewdesc.Texture2DArray.ArraySize = 1;
		viewdesc.Texture2D.MipLevels = m_tDesc.MipLevels;
		viewdesc.Texture2D.MostDetailedMip = 0;
	}

	// 텍스쳐의 View 생성
	hRet = DEVICE->CreateShaderResourceView(m_pTex2D.Get(), &viewdesc, m_pSRV.GetAddressOf());

	if (FAILED(hRet)) {
		_tprintf_s(strBuff, _T("에러코드 : %d"), hRet);
		MessageBox(nullptr, strBuff, STR_MSG_FailedToCreateShaderResourceView, MB_OK);
		return hRet;
	}

	// 밉맵 생성
	if (false == m_Image.GetMetadata().IsCubemap())
		CONTEXT->GenerateMips(m_pSRV.Get());

	//// 리소스 가져오기
	//m_pSRV->GetResource((ID3D11Resource**)m_pTex2D.GetAddressOf());

	// 텍스쳐 정보 저장
	m_pTex2D->GetDesc(&m_tDesc);

	return hRet;
}

bool CTexture::Save(const tstring& _strRelativePath)
{
	CaptureTexture(DEVICE.Get(), CONTEXT.Get(), m_pTex2D.Get(), m_Image);

	HRESULT hr = S_OK;

	wstring strPath = CPathManager::GetInstance()->GetContentPath();
	strPath += _strRelativePath;

	size_t arrsize = m_Image.GetMetadata().arraySize;


	// 밉맵이 1 이상인 경우
	if (1 < m_tDesc.MipLevels) {
		D3D11_TEXTURE2D_DESC texdesc = {};
		texdesc = m_tDesc;
		texdesc.MipLevels = 1;

		ComPtr<ID3D11Texture2D> pCopyTex2D;
		HRESULT hr = DEVICE->CreateTexture2D(&texdesc, nullptr, pCopyTex2D.GetAddressOf());

		for (int i = 0; i < (int)m_tDesc.ArraySize; ++i) {
			// m_tex2D ==> pCopyTex2D
			UINT iDestSubresIdx = D3D11CalcSubresource(0, i, 1);
			UINT iSrcSubresIdx = D3D11CalcSubresource(0, i, MAX_MIP);

			CONTEXT->CopySubresourceRegion(pCopyTex2D.Get(), iDestSubresIdx, 0, 0, 0
				, m_pTex2D.Get(), iSrcSubresIdx, nullptr);
		}

		ScratchImage tempimage;
		CaptureTexture(DEVICE.Get(), CONTEXT.Get(), pCopyTex2D.Get(), tempimage);

		if (1 < arrsize) {
			hr = SaveToDDSFile(tempimage.GetImages(), arrsize, tempimage.GetMetadata(), DDS_FLAGS::DDS_FLAGS_NONE, strPath.c_str());
		}
		else {
			hr = SaveToWICFile(*(tempimage.GetImages())
				, WIC_FLAGS_NONE
				, GetWICCodec(static_cast<WICCodecs>(WICCodecs::WIC_CODEC_PNG))
				, strPath.c_str());
		}
	}

	// 밉맵이 1 레벨(원본만) 있는 경우, 캡쳐 후 바로 저장
	else {
		if (1 < arrsize) {
			hr = SaveToDDSFile(m_Image.GetImages(), arrsize, m_Image.GetMetadata(), DDS_FLAGS::DDS_FLAGS_NONE, strPath.c_str());
		}
		else {
			hr = SaveToWICFile(*(m_Image.GetImages())
				, WIC_FLAGS_NONE
				, GetWICCodec(static_cast<WICCodecs>(WICCodecs::WIC_CODEC_PNG))
				, strPath.c_str());
		}
	}

	if (FAILED(hr)) {
		assert(nullptr && _T("Texture save error"));
		return false;
	}

	return true;
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

void CTexture::CreateArrayTexture(const vector<SharedPtr<CTexture>>& _vecTex, int _iMapLevel)
{
	m_tDesc = _vecTex[0]->GetDesc();
	m_tDesc.ArraySize = (UINT)_vecTex.size();
	m_tDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	m_tDesc.MipLevels = _iMapLevel;

	HRESULT hr = DEVICE->CreateTexture2D(&m_tDesc, nullptr, m_pTex2D.GetAddressOf());
	if (FAILED(hr)) {
		assert(nullptr && _T("Array Texture 생성 Error"));
		return;
	}

	// 원본데이터(밉맵 레벨 0) 를 각 칸에 옮긴다.	
	for (int i = 0; i < _vecTex.size(); ++i) {
		UINT iOffset = D3D11CalcSubresource(0, i, _iMapLevel);
		CONTEXT->UpdateSubresource(m_pTex2D.Get(), iOffset, nullptr, _vecTex[i]->GetSysMem()
			, _vecTex[i]->GetRowPitch(), _vecTex[i]->GetSlicePitch());
	}

	// Shader Resource View 생성
	D3D11_SHADER_RESOURCE_VIEW_DESC viewdesc = {};
	viewdesc.Format = m_tDesc.Format;
	viewdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	viewdesc.Texture2DArray.MipLevels = _iMapLevel;
	viewdesc.Texture2DArray.MostDetailedMip = 0;
	viewdesc.Texture2DArray.ArraySize = _vecTex.size();

	DEVICE->CreateShaderResourceView(m_pTex2D.Get(), &viewdesc, m_pSRV.GetAddressOf());

	// 밉맵 생성
	CONTEXT->GenerateMips(m_pSRV.Get());
	m_pTex2D->GetDesc(&m_tDesc);
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