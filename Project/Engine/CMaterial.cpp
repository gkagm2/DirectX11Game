#include "pch.h"
#include "CMaterial.h"
#include "CConstBuffer.h"
#include "CDevice.h"
#include "CTexture.h"

CMaterial::CMaterial() :
	m_pShader(nullptr),
	m_tParam{},
	m_arrTexture{}
{
}

CMaterial::~CMaterial()
{
}

int CMaterial::Load(const tstring& _strFilePath)
{
	return 0;
}

void CMaterial::UpdateData()
{
	m_pShader->UpdateData();

	// ��� ������ ������Ʈ

	static const CConstBuffer* pCB = CDevice::GetInstance()->GetConstBuffer(E_ConstBuffer::Material_Param);
	pCB->SetData(&m_tParam);
	pCB->UpdateData();

	// �ؽ���
	for (UINT i = 0; i < (UINT)E_ShaderParam::Texture_End - (UINT)E_ShaderParam::Texture_0; ++i) {
		if (nullptr != m_arrTexture[i])
			m_arrTexture[i]->UpdateData(E_ShaderStage::All, i);
		else
			CTexture::Clear(i);
	}
}

void CMaterial::SetData(E_ShaderParam _eParam, void* _pData)
{
	switch (_eParam) {
	case E_ShaderParam::Int_0:
	case E_ShaderParam::Int_1:
	case E_ShaderParam::Int_2:
	case E_ShaderParam::Int_3:
		m_tParam.iArr[(UINT)_eParam - (UINT)E_ShaderParam::Int_0] = *((int*)_pData);
		break;

	case E_ShaderParam::Float_0:
	case E_ShaderParam::Float_1:
	case E_ShaderParam::Float_2:
	case E_ShaderParam::Float_3:
		m_tParam.fArr[(UINT)_eParam - (UINT)E_ShaderParam::Float_0] = *((float*)_pData);
		break;

	case E_ShaderParam::Vector2_0:
	case E_ShaderParam::Vector2_1:
	case E_ShaderParam::Vector2_2:
	case E_ShaderParam::Vector2_3:
		m_tParam.v2Arr[(UINT)_eParam - (UINT)E_ShaderParam::Vector2_0] = *((Vector2*)_pData);
		break;

	case E_ShaderParam::Vector4_0:
	case E_ShaderParam::Vector4_1:
	case E_ShaderParam::Vector4_2:
	case E_ShaderParam::Vector4_3:
		m_tParam.v4Arr[(UINT)_eParam - (UINT)E_ShaderParam::Vector4_0] = *((Vector4*)_pData);
		break;

	case E_ShaderParam::Matrix_0:
	case E_ShaderParam::Matrix_1:
	case E_ShaderParam::Matrix_2:
	case E_ShaderParam::Matrix_3:
		m_tParam.matArr[(UINT)_eParam - (UINT)E_ShaderParam::Matrix_0] = *((Matrix*)_pData);
		break;

	case E_ShaderParam::Texture_0:
	case E_ShaderParam::Texture_1:
	case E_ShaderParam::Texture_2:
	case E_ShaderParam::Texture_3:
	case E_ShaderParam::Texture_4:
	case E_ShaderParam::Texture_5:
	case E_ShaderParam::Texture_6:
	case E_ShaderParam::Texture_7:
	case E_ShaderParam::TextureArr_0:
	case E_ShaderParam::TextureArr_1:
	case E_ShaderParam::TextureArr_2:
	case E_ShaderParam::TextureCube_0:
	case E_ShaderParam::TextureCube_1:
	case E_ShaderParam::TextureCube_2:
		m_arrTexture[(UINT)_eParam - (UINT)E_ShaderParam::Texture_0] = (CTexture*)_pData;

		break;
	}
}