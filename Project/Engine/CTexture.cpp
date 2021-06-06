#include "pch.h"
#include "CTexture.h"
#include "CDevice.h"

CTexture::CTexture()
{
}

CTexture::~CTexture()
{
}

int CTexture::Load(const tstring& _strFilePath)
{
	TCHAR strBuff[50] = {};
	_tsplitpath_s(_strFilePath.c_str(), 0, 0, 0, 0, 0, 0, strBuff, 50);
	tstring strExt = strBuff;

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

	return hRet;
}

void CTexture::UpdateData(E_ShaderStage _eShaderStage, UINT _iRegisterNum)
{
	if((UINT)E_ShaderStage::Vertex & (UINT)_eShaderStage)
		CONTEXT->VSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());
	else if ((UINT)E_ShaderStage::Hull & (UINT)_eShaderStage)
		CONTEXT->HSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());
	else if ((UINT)E_ShaderStage::Domain & (UINT)_eShaderStage)
		CONTEXT->DSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());
	else if ((UINT)E_ShaderStage::Geometry & (UINT)_eShaderStage)
		CONTEXT->GSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());
	else if ((UINT)E_ShaderStage::Pixel & (UINT)_eShaderStage)
		CONTEXT->PSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());
	else if ((UINT)E_ShaderStage::Compute & (UINT)_eShaderStage)
		CONTEXT->CSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());
}