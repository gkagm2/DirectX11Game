#include "pch.h"
#include "CGraphicsShader.h"
#include "CPathManager.h"
#include "CDevice.h"

CGraphicsShader::CGraphicsShader() :
	m_eTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
}

CGraphicsShader::~CGraphicsShader()
{
}

void CGraphicsShader::UpdateData()
{
	//// Topology(위상구조) 설정
	CONTEXT->IASetPrimitiveTopology(m_eTopology); // 삼각형

	//// InputLayout(Semantic) 설정
	CONTEXT->IASetInputLayout(m_pLayout.Get());

	//// 장치가 사용 할 VertexShader, PixelShader 세팅
	CONTEXT->VSSetShader(m_pVS.Get(), nullptr, 0);
	CONTEXT->PSSetShader(m_pPS.Get(), nullptr, 0);
}

void CGraphicsShader::CreateVertexShader(const tstring& _strRelativePath, const string& _strFuncName)
{
	tstring strShaderFilePath = CPathManager::GetInstance()->GetContentPath();
	strShaderFilePath += _strRelativePath;

	int iFlag = D3DCOMPILE_DEBUG;

	if (FAILED(D3DCompileFromFile(strShaderFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFuncName.c_str(), "vs_5_0", iFlag, 0, m_pVSBlob.GetAddressOf(), m_pErrBlob.GetAddressOf()))) {
		char* pErrorMessage = (char*)m_pErrBlob->GetBufferPointer();
		MessageBoxA(nullptr, pErrorMessage, STR_MSG_FailedToCreateVertexShader, MB_OK);
		assert(nullptr);
	}

	if (FAILED(DEVICE->CreateVertexShader(m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize(), nullptr, m_pVS.GetAddressOf()))) {
		assert(nullptr);
	}

	// InputLayout(Sementic) 설정
	DEVICE->CreateInputLayout(g_arrLayoutDesc, LAYOUT_DESCRIPTION_COUNT, m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize(), m_pLayout.GetAddressOf());
}

void CGraphicsShader::CreatePixelShader(const tstring& _strRelativePath, const string& _strFuncName)
{
	tstring strShaderFilePath = CPathManager::GetInstance()->GetContentPath();
	strShaderFilePath += _strRelativePath;

	int iFlag = D3DCOMPILE_DEBUG;

	if (FAILED(D3DCompileFromFile(strShaderFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFuncName.c_str(), "ps_5_0", iFlag, 0, m_pPSBlob.GetAddressOf(), m_pErrBlob.GetAddressOf()))) {
		char* pErrorMessage = (char*)m_pErrBlob->GetBufferPointer();
		MessageBoxA(nullptr, pErrorMessage, STR_MSG_FailedToCreatePixelShader, MB_OK);
		assert(nullptr);
	}

	if (FAILED(DEVICE->CreatePixelShader(m_pPSBlob->GetBufferPointer(), m_pPSBlob->GetBufferSize(), nullptr, m_pPS.GetAddressOf()))) {
		assert(nullptr);
	}
}