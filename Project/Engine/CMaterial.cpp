#include "pch.h"
#include "CMaterial.h"
#include "CConstBuffer.h"
#include "CDevice.h"
#include "CTexture.h"

#include "CSceneManager.h"
#include "CResourceManager.h"

CMaterial::CMaterial() :
	CResource(E_ResourceType::Material),
	m_pShader(nullptr),
	m_tParam{},
	m_arrTexture{},
	m_bIsDefaultMtrl(false),
	m_bIsCloneMtrlInnerEngine(false)
{
}

CMaterial::CMaterial(const CMaterial& _origin) :
	CResource(E_ResourceType::Material),
	m_pShader(_origin.m_pShader),
	m_tParam{_origin.m_tParam},
	m_arrTexture{},
	m_bIsDefaultMtrl(false),
	m_bIsCloneMtrlInnerEngine(_origin.m_bIsCloneMtrlInnerEngine)
{	
	memcpy(m_arrTexture, _origin.m_arrTexture, sizeof(_origin.m_arrTexture));
}

CMaterial::CMaterial(bool _bIsDefaultMaterial) :
	CResource(E_ResourceType::Material),
	m_pShader(nullptr),
	m_tParam{},
	m_arrTexture{},
	m_bIsDefaultMtrl(_bIsDefaultMaterial),
	m_bIsCloneMtrlInnerEngine(false)
{
}

CMaterial::~CMaterial()
{
}

void CMaterial::UpdateData()
{
	if (!m_pShader.Get())
		return;
	m_pShader->UpdateData();

	// 텍스쳐
	for (UINT i = 0; i < (UINT)E_ShaderParam::Texture_End - (UINT)E_ShaderParam::Texture_0; ++i) {
		if (nullptr != m_arrTexture[i]) {
			m_arrTexture[i]->UpdateData(E_ShaderStage::All, i);
			m_tParam.bTexArr[i] = true;
		}
		else {
			CTexture::Clear(i); // texture register number == idx
			m_tParam.bTexArr[i] = false;
		}
	}

	// 상수 데이터 업데이트
	static const CConstBuffer* pCB = CDevice::GetInstance()->GetConstBuffer(E_ConstBuffer::Material_Param);
	pCB->SetData(&m_tParam);
	pCB->UpdateData();
}


void CMaterial::SetShader(SharedPtr<CGraphicsShader>& _pShader) {
	m_pShader = _pShader;
	// 엔진 기본 메터리얼이 아니여야 하고, Scene이 StopMode일때만 메터리얼의 변경점을 저장.
	if (!m_bIsDefaultMtrl && E_SceneMode::Stop == CSceneManager::GetInstance()->GetSceneMode()) {
		if(false == m_bIsCloneMtrlInnerEngine)
			Save(GetRelativePath());
	}
}

// Example :
// SetData(E_ShaderParam::Texture_0, pTestTexture.Get());
void CMaterial::SetData(E_ShaderParam _eParam, const void* _pData)
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
	case E_ShaderParam::TextureCube_0:
	case E_ShaderParam::TextureCube_1:
		m_arrTexture[(UINT)_eParam - (UINT)E_ShaderParam::Texture_0] = (CTexture*)_pData;

		break;
	default:
		assert(nullptr);
		break;
	}

	// 엔진 기본 메터리얼이 아니여야 하고, Scene이 StopMode일때만 메터리얼의 변경점을 저장.
	if (!m_bIsDefaultMtrl && E_SceneMode::Stop == CSceneManager::GetInstance()->GetSceneMode()) {
		if(false == m_bIsCloneMtrlInnerEngine)
			Save(GetRelativePath());
	}
		
}

// Example : 1
// CTexture* pTex = nullptr;
// GetData(E_ShaderParam::Texture_0, &pTex);
// SharedPtr<CTexture> pTex;
// GetData(E_ShaderParam::Texture_0, pTex.GetAddress());
void CMaterial::GetData(E_ShaderParam _eParam, void* _pOut)
{
	switch (_eParam) {
	case E_ShaderParam::Int_0:
	case E_ShaderParam::Int_1:
	case E_ShaderParam::Int_2:
	case E_ShaderParam::Int_3:
		*((int*)_pOut) = m_tParam.iArr[(UINT)_eParam - (UINT)E_ShaderParam::Int_0];
		break;

	case E_ShaderParam::Float_0:
	case E_ShaderParam::Float_1:
	case E_ShaderParam::Float_2:
	case E_ShaderParam::Float_3:
		*((float*)_pOut) = m_tParam.fArr[(UINT)_eParam - (UINT)E_ShaderParam::Float_0];
		break;

	case E_ShaderParam::Vector2_0:
	case E_ShaderParam::Vector2_1:
	case E_ShaderParam::Vector2_2:
	case E_ShaderParam::Vector2_3:
		*((Vector2*)_pOut) = m_tParam.v2Arr[(UINT)_eParam - (UINT)E_ShaderParam::Vector2_0];
		break;

	case E_ShaderParam::Vector4_0:
	case E_ShaderParam::Vector4_1:
	case E_ShaderParam::Vector4_2:
	case E_ShaderParam::Vector4_3:
		*((Vector4*)_pOut) = m_tParam.v4Arr[(UINT)_eParam - (UINT)E_ShaderParam::Vector4_0];
		break;

	case E_ShaderParam::Matrix_0:
	case E_ShaderParam::Matrix_1:
	case E_ShaderParam::Matrix_2:
	case E_ShaderParam::Matrix_3:
		*((Matrix*)_pOut) = m_tParam.matArr[(UINT)_eParam - (UINT)E_ShaderParam::Matrix_0];
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
	case E_ShaderParam::TextureCube_0:
	case E_ShaderParam::TextureCube_1:
		(*(CTexture**)_pOut) = m_arrTexture[(UINT)_eParam - (UINT)E_ShaderParam::Texture_0].Get();
		break;
	default:
		assert(nullptr);
		break;
	}
}

bool CMaterial::Save(const tstring& _strRelativePath)
{
	tstring strFilePath = CPathManager::GetInstance()->GetContentPath();
	strFilePath += _strRelativePath;

	FILE* pFile = nullptr;
	errno_t err;
	err = _tfopen_s(&pFile, strFilePath.c_str(), _T("wb"));
	assert(pFile);
	if (err)
		return false;
	
	SaveResourceToFile(m_pShader, pFile);
	FWrite(m_tParam, pFile);

	UINT iMaxTex = (UINT)E_ShaderParam::Texture_End - (UINT)E_ShaderParam::Texture_0;
	for (UINT i = 0; i < iMaxTex; ++i)
		SaveResourceToFile(m_arrTexture[i], pFile);

	FWrite(m_bIsDefaultMtrl, pFile);
	FWrite(m_bIsCloneMtrlInnerEngine, pFile);

	fclose(pFile);
	return true;
}

int CMaterial::Load(const tstring& _strFilePath)
{
	FILE* pFile = nullptr;
	errno_t err;
	err = _tfopen_s(&pFile, _strFilePath.c_str(), _T("rb"));
	assert(pFile);
	if (err)
		return false;

	LoadResourceFromFile(m_pShader, pFile);
	FRead(m_tParam, pFile);

	UINT iMaxTex = (UINT)E_ShaderParam::Texture_End - (UINT)E_ShaderParam::Texture_0;
	for (UINT i = 0; i < iMaxTex; ++i)
		LoadResourceFromFile(m_arrTexture[i], pFile);

	FRead(m_bIsDefaultMtrl, pFile);
	FRead(m_bIsCloneMtrlInnerEngine, pFile);

	fclose(pFile);
	return S_OK;
}

void CMaterial::Clear()
{
	CGraphicsShader::Clear();
	for (UINT i = 0; i < (UINT)E_ShaderParam::Texture_End - (UINT)E_ShaderParam::Texture_0; ++i)
		CTexture::Clear(i);
}

CMaterial* CMaterial::Clone()
{
	SharedPtr<CMaterial> pMtrl = new CMaterial(*this);
	CResourceManager::GetInstance()->AddCloneMaterial(pMtrl);
	return pMtrl.Get();
}

CMaterial* CMaterial::Clone_NoAddInResMgr()
{
	SharedPtr<CMaterial> pMtrl = new CMaterial(*this);
	pMtrl->_SetCloneMtrlInnerEngineFlag();
	static int i = 0;
	tstring strKey = _T("MtrlClone") + to_tstring(i++) + _T(".mtrl");
	pMtrl->SetKey(strKey);
	pMtrl->SetRelativePath(strKey);
	return pMtrl.Get();
}
