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
	TCHAR strBuff[250] = {};
	_tsplitpath_s(_strFilePath.c_str(), 0, 0, 0, 0, 0, 0, strBuff, 250);
	tstring strExt = strBuff;

	HRESULT hRet = S_OK;
	if (strExt == _T(".dds") || strExt == _T(".DDS")) {
		// .dds .DDS ����
		hRet = LoadFromDDSFile(_strFilePath.c_str(), DDS_FLAGS_NONE, nullptr, m_Image);
	}
	else if (strExt == _T(".tga") || strExt == _T(".TGA")) {
		// .tga .TGA ����
		hRet = LoadFromTGAFile(_strFilePath.c_str(), nullptr, m_Image);
	}
	else {
		// png, jpg, jpeg, bmp ����
		hRet = LoadFromWICFile(_strFilePath.c_str(), WIC_FLAGS_NONE, nullptr, m_Image);
	}

	if (FAILED(hRet)) {
		_tprintf_s(strBuff, _T("�����ڵ� : %d"), hRet);
		MessageBox(nullptr, strBuff, STR_MSG_FailedToLoadTexture, MB_OK);
		return hRet;
	}

	// �ؽ����� View ����
	hRet = CreateShaderResourceView(
		DEVICE.Get(),
		m_Image.GetImages(),
		m_Image.GetImageCount(),
		m_Image.GetMetadata(),
		m_pSRV.GetAddressOf());

	if (FAILED(hRet)) {
		_tprintf_s(strBuff, _T("�����ڵ� : %d"), hRet);
		MessageBox(nullptr, strBuff, STR_MSG_FailedToCreateShaderResourceView, MB_OK);
		return hRet;
	}

	// ���ҽ� ��������
	m_pSRV->GetResource((ID3D11Resource**)m_pTex2D.GetAddressOf());

	return hRet;
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
