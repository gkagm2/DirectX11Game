#include "pch.h"
#include "CGraphicsShader.h"
#include "CPathManager.h"
#include "CDevice.h"

CGraphicsShader::CGraphicsShader() :
	CShader(E_ResourceType::GraphicsShader),
	m_eTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
	m_eRSState(E_RasterizerState::CullBack),
	m_eBlendState(E_BlendState::Default),
	m_eDepthStencilState(E_DepthStencilState::Less),
	m_eRenderTimePoint(E_RenderTimePoint::Forward)
{
}

CGraphicsShader::CGraphicsShader(E_RenderTimePoint _eTimePoint) :
	CShader(E_ResourceType::GraphicsShader),
	m_eTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
	m_eRSState(E_RasterizerState::CullBack),
	m_eBlendState(E_BlendState::Default),
	m_eDepthStencilState(E_DepthStencilState::Less),
	m_eRenderTimePoint(_eTimePoint)
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

	//// 장치가 사용 할 Shader 세팅
	CONTEXT->VSSetShader(m_pVS.Get(), 0, 0);
	CONTEXT->HSSetShader(m_pHS.Get(), 0, 0);
	CONTEXT->DSSetShader(m_pDS.Get(), 0, 0);
	CONTEXT->GSSetShader(m_pGS.Get(), 0, 0);
	CONTEXT->PSSetShader(m_pPS.Get(), 0, 0);

	CDevice::GetInstance()->SetRasterizerState(m_eRSState);
	CDevice::GetInstance()->SetBlendState(m_eBlendState);
	CDevice::GetInstance()->SetDepthStencilState(m_eDepthStencilState);
}

void CGraphicsShader::Clear()
{
	ID3D11VertexShader*		pVS = nullptr;
	ID3D11HullShader*		pHS = nullptr;
	ID3D11DomainShader*		pDS = nullptr;
	ID3D11GeometryShader*	pGS = nullptr;
	ID3D11PixelShader*		pPS = nullptr;

	CONTEXT->VSSetShader(pVS, 0, 0);
	CONTEXT->HSSetShader(pHS, 0, 0);
	CONTEXT->DSSetShader(pDS, 0, 0);
	CONTEXT->GSSetShader(pGS, 0, 0);
	CONTEXT->PSSetShader(pPS, 0, 0);
}

void CGraphicsShader::CreateVertexShader(const tstring& _strRelativePath, const string& _strFuncName)
{
	tstring strShaderFilePath = CPathManager::GetInstance()->GetContentPath();
	strShaderFilePath += _strRelativePath;

	UINT iFlag = D3DCOMPILE_DEBUG;

	if (FAILED(D3DCompileFromFile(strShaderFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFuncName.c_str(), "vs_5_0", iFlag, 0, m_pVSBlob.GetAddressOf(), m_pErrBlob.GetAddressOf()))) {
		char* pErrorMessage = (char*)m_pErrBlob->GetBufferPointer();
		MessageBoxA(nullptr, pErrorMessage, STR_MSG_FailedToCreateVertexShader, MB_OK);
		assert(nullptr);
	}
	if (m_pErrBlob.Get()) {
		char* pErrorMessage = (char*)m_pErrBlob->GetBufferPointer();
		MessageBoxA(nullptr, pErrorMessage, STR_MSG_FailedToCreateVertexShader, MB_OK);
		assert(nullptr);
	}

	if (FAILED(DEVICE->CreateVertexShader(m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize(), nullptr, m_pVS.GetAddressOf()))) {
		assert(nullptr);
	}

	// InputLayout(Sementic) 설정
	UINT iElementsCnt = (UINT)g_vecLayoutDesc.size();

	if (FAILED(DEVICE->CreateInputLayout(g_vecLayoutDesc.data(), iElementsCnt, m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize(), m_pLayout.GetAddressOf()))) {
		assert(nullptr);
	}
}

void CGraphicsShader::CreateHullShader(const tstring& _strRelativePath, const string& _strFuncName)
{
	tstring strShaderFilePath = CPathManager::GetInstance()->GetContentPath();
	strShaderFilePath += _strRelativePath;

	UINT iFlag = D3DCOMPILE_DEBUG;

	// D3DCompileFromFile -> D3DCompile2
	if (FAILED(D3DCompileFromFile(strShaderFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFuncName.c_str(), "hs_5_0", iFlag, 0, m_pHSBlob.GetAddressOf(), m_pErrBlob.GetAddressOf()))) {
		char* pErrorMessage = (char*)m_pErrBlob->GetBufferPointer();
		MessageBoxA(nullptr, pErrorMessage, STR_MSG_FailedToCreateHullShader, MB_OK);
		assert(nullptr);
	}
	if (m_pErrBlob.Get()) {
		char* pErrorMessage = (char*)m_pErrBlob->GetBufferPointer();
		MessageBoxA(nullptr, pErrorMessage, STR_MSG_FailedToCreateHullShader, MB_OK);
		assert(nullptr);
	}
	if (FAILED(DEVICE->CreateHullShader(m_pHSBlob->GetBufferPointer(), m_pHSBlob->GetBufferSize(), nullptr, m_pHS.GetAddressOf()))) {
		assert(nullptr);
	}
}

void CGraphicsShader::CreateDomainShader(const tstring& _strRelativePath, const string& _strFuncName)
{
	tstring strShaderFilePath = CPathManager::GetInstance()->GetContentPath();
	strShaderFilePath += _strRelativePath;

	UINT iFlag = D3DCOMPILE_DEBUG;

	// D3DCompileFromFile -> D3DCompile2
	if (FAILED(D3DCompileFromFile(strShaderFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFuncName.c_str(), "ds_5_0", iFlag, 0, m_pDSBlob.GetAddressOf(), m_pErrBlob.GetAddressOf()))) {
		char* pErrorMessage = (char*)m_pErrBlob->GetBufferPointer();
		MessageBoxA(nullptr, pErrorMessage, STR_MSG_FailedToCreateDomainShader, MB_OK);
		assert(nullptr);
	}
	if (m_pErrBlob.Get()) {
		char* pErrorMessage = (char*)m_pErrBlob->GetBufferPointer();
		MessageBoxA(nullptr, pErrorMessage, STR_MSG_FailedToCreateDomainShader, MB_OK);
		assert(nullptr);
	}
	if (FAILED(DEVICE->CreateDomainShader(m_pDSBlob->GetBufferPointer(), m_pDSBlob->GetBufferSize(), nullptr, m_pDS.GetAddressOf()))) {
		assert(nullptr);
	}
}

void CGraphicsShader::CreateGeometryShader(const tstring& _strRelativePath, const string& _strFuncName)
{
	tstring strShaderFilePath = CPathManager::GetInstance()->GetContentPath();
	strShaderFilePath += _strRelativePath;

	UINT iFlag = D3DCOMPILE_DEBUG;

	// D3DCompileFromFile -> D3DCompile2
	if (FAILED(D3DCompileFromFile(strShaderFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFuncName.c_str(), "gs_5_0", iFlag, 0, m_pGSBlob.GetAddressOf(), m_pErrBlob.GetAddressOf()))) {
		char* pErrorMessage = (char*)m_pErrBlob->GetBufferPointer();
		MessageBoxA(nullptr, pErrorMessage, STR_MSG_FailedToCreateGeometryShader, MB_OK);
		assert(nullptr);
	}
	if (m_pErrBlob.Get()) {
		char* pErrorMessage = (char*)m_pErrBlob->GetBufferPointer();
		MessageBoxA(nullptr, pErrorMessage, STR_MSG_FailedToCreateGeometryShader, MB_OK);
		assert(nullptr);
	}

	if (FAILED(DEVICE->CreateGeometryShader(m_pGSBlob->GetBufferPointer(), m_pGSBlob->GetBufferSize()	 , nullptr, m_pGS.GetAddressOf()))) {
		assert(nullptr);
	}
}

void CGraphicsShader::CreatePixelShader(const tstring& _strRelativePath, const string& _strFuncName)
{
	tstring strShaderFilePath = CPathManager::GetInstance()->GetContentPath();
	strShaderFilePath += _strRelativePath;

	UINT iFlag = D3DCOMPILE_DEBUG;

	// D3DCompileFromFile -> D3DCompile2
	if (FAILED(D3DCompileFromFile(strShaderFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFuncName.c_str(), "ps_5_0", iFlag, 0, m_pPSBlob.GetAddressOf(), m_pErrBlob.GetAddressOf()))) {
		char* pErrorMessage = (char*)m_pErrBlob->GetBufferPointer();
		MessageBoxA(nullptr, pErrorMessage, STR_MSG_FailedToCreatePixelShader, MB_OK);
		assert(nullptr);
	}

	if (m_pErrBlob.Get()) {
		char* pErrorMessage = (char*)m_pErrBlob->GetBufferPointer();
		MessageBoxA(nullptr, pErrorMessage, STR_MSG_FailedToCreatePixelShader, MB_OK);
		assert(nullptr);
	}

	if (FAILED(DEVICE->CreatePixelShader(m_pPSBlob->GetBufferPointer(), m_pPSBlob->GetBufferSize(), nullptr, m_pPS.GetAddressOf()))) {
		assert(nullptr);
	}
}