#include "pch.h"
#include "CPathManager.h"
#include "CComputeShader.h"
#include "CDevice.h"

const CConstBuffer* CComputeShader::g_pMtrlBuffer = nullptr;
CComputeShader::CComputeShader(UINT _iThreadNumX, UINT _iThreadNumY, UINT _iThreadNumZ) :
	CShader(E_ResourceType::ComputeShader),
	m_iThreadNumX(_iThreadNumX),
	m_iThreadNumY(_iThreadNumY),
	m_iThreadNumZ(_iThreadNumZ),
	m_tInfo{}
{
	if (nullptr == g_pMtrlBuffer)
		g_pMtrlBuffer = CDevice::GetInstance()->GetConstBuffer(E_ConstBuffer::Material_Param);
}

CComputeShader::~CComputeShader()
{
}

void CComputeShader::CreateComputeShader(const tstring& _strRelativePath, const string& _strFuncName)
{
	tstring strShaderFile = CPathManager::GetInstance()->GetContentPath() + _strRelativePath;

	int iFlag = D3DCOMPILE_DEBUG;

	if (FAILED(D3DCompileFromFile(strShaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFuncName.c_str(), "cs_5_0", iFlag, 0, m_pCSBlob.GetAddressOf(), m_pErrBlob.GetAddressOf()))) {
		char* pErrorMessage = (char*)m_pErrBlob->GetBufferPointer();
		MessageBoxA(nullptr, pErrorMessage, STR_MSG_FailedToCreateComputeShader, MB_OK);
		assert(nullptr);
	}

	if (FAILED(DEVICE->CreateComputeShader(m_pCSBlob->GetBufferPointer(), m_pCSBlob->GetBufferSize(), nullptr, m_pCS.GetAddressOf()))) {
		assert(nullptr);
	}
}

void CComputeShader::Dispatch(UINT _x, UINT _y, UINT _z)
{
	UpdateData();
	CONTEXT->CSSetShader(m_pCS.Get(), 0, 0);
	CONTEXT->Dispatch(_x, _y, _z); // CS ½ÇÇà

	Clear();
}

