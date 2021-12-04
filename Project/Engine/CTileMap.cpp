#include "pch.h"
#include "CTileMap.h"
#include "CResourceManager.h"
#include "CTransform.h"
#include "CStructuredBuffer.h"
#include "CRenderManager.h"

CTileMap::CTileMap() :
	CComponent(E_ComponentType::TileMap),
	m_pTileMapBuffer(nullptr),
	m_iAtlasTileXCnt(5),
	m_iAtlasTileYCnt(5),
	m_iTileXCnt(2),
	m_iTileYCnt(2),
	m_iDefaultTileColCnt(2),
	m_iDefaultTileRowCnt(2)
{
	m_pMesh = CResourceManager::GetInstance()->LoadRes<CMesh>(STR_KEY_RectMesh);
	m_pMaterial = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_TileMapMtrl);

	m_vecTileInfo.resize(m_iDefaultTileColCnt * m_iDefaultTileRowCnt);
	for (int i = 0; i < m_vecTileInfo.size(); ++i)
		m_vecTileInfo[i].idx = 1;


	m_pTileMapBuffer = make_unique<CStructuredBuffer>();
	m_pTileMapBuffer->Create(E_StructuredBufferType::ReadOnly, sizeof(TTileInfo), m_vecTileInfo.size());
}

CTileMap::CTileMap(const CTileMap& _origin) :
	CComponent(E_ComponentType::TileMap),
	m_pTileMapBuffer(nullptr),
	m_iAtlasTileXCnt(_origin.m_iAtlasTileXCnt),
	m_iAtlasTileYCnt(_origin.m_iAtlasTileYCnt),
	m_iTileXCnt(_origin.m_iTileXCnt),
	m_iTileYCnt(_origin.m_iTileYCnt),
	m_iDefaultTileColCnt(_origin.m_iDefaultTileColCnt),
	m_iDefaultTileRowCnt(_origin.m_iDefaultTileRowCnt),
	m_pMesh(_origin.m_pMesh),
	m_pMaterial(_origin.m_pMaterial)
{
	m_pTileMapBuffer = make_unique<CStructuredBuffer>();

	m_vecTileInfo.resize(m_iDefaultTileColCnt * m_iDefaultTileRowCnt);
	for (int i = 0; i < m_vecTileInfo.size(); ++i)
		m_vecTileInfo[i].idx = _origin.m_vecTileInfo[i].idx;

	m_pTileMapBuffer->Create(E_StructuredBufferType::ReadOnly, sizeof(TTileInfo), m_vecTileInfo.size(), m_vecTileInfo.data());
}

CTileMap::~CTileMap()
{
}

void CTileMap::FinalUpdate()
{
	CRenderManager::GetInstance()->RegisterTileMap(this);
}

void CTileMap::UpdateData()
{
	// -- 생성할 타일의 개수 세팅
	m_pMaterial->SetData(E_ShaderParam::Int_0, &m_iTileXCnt);
	m_pMaterial->SetData(E_ShaderParam::Int_1, &m_iTileYCnt);

	// -- 아틀라스 텍스쳐 세팅
	m_pMaterial->SetData(E_ShaderParam::Texture_0, m_pAtlasTexture.Get());

	// -- 아틀라스 텍스쳐의 사이즈
	Vector2 vAtlasResolution = m_pAtlasTexture->GetDimension();
	m_pMaterial->SetData(E_ShaderParam::Vector2_0, &vAtlasResolution);

	// -- 아틀라스 텍스쳐에서 타일 하나의 UV 사이즈
	Vector2 vAtlasTileSize = {};
	try {
		vAtlasTileSize = vAtlasResolution / Vector2(m_iAtlasTileXCnt, m_iAtlasTileYCnt);
	}
	catch (std::exception e) {
		vAtlasTileSize = Vector2(0.f, 0.f);
	}
	Vector2 vAtlasTileUVSize = vAtlasTileSize / vAtlasResolution;
	m_pMaterial->SetData(E_ShaderParam::Vector2_1, &vAtlasTileUVSize);

	Transform()->UpdateData();
	m_pMaterial->UpdateData();

	m_pTileMapBuffer->SetData(m_vecTileInfo.data(), (UINT)m_vecTileInfo.size());
	m_pTileMapBuffer->UpdateData(REGISTER_NUM_TileMapBuffer, E_ShaderStage::Pixel);
}

void CTileMap::Render()
{
	UpdateData();

	m_pMesh->Render();
	m_pMaterial->Clear();
}

void CTileMap::_InsertTileInfoToBuffer()
{
	m_pTileMapBuffer->Create(E_StructuredBufferType::ReadOnly, sizeof(TTileInfo), m_vecTileInfo.size(), m_vecTileInfo.data());
}

bool CTileMap::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);
	SaveResourceToFile(m_pMesh, _pFile);
	SaveResourceToFile(m_pMaterial, _pFile);
	SaveResourceToFile(m_pAtlasTexture, _pFile);

	size_t iTileInfoCnt = m_vecTileInfo.size();
	FWrite(iTileInfoCnt, _pFile);
	for (UINT i = 0; i < iTileInfoCnt; ++i)
		FWrite(m_vecTileInfo[i], _pFile);

	FWrite(m_iTileXCnt, _pFile);
	FWrite(m_iTileYCnt, _pFile);
	FWrite(m_iAtlasTileXCnt, _pFile);
	FWrite(m_iAtlasTileYCnt, _pFile);

	return true;
}

bool CTileMap::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);
	LoadResourceFromFile(m_pMesh, _pFile);
	LoadResourceFromFile(m_pMaterial, _pFile);
	LoadResourceFromFile(m_pAtlasTexture, _pFile);

	size_t iTileInfoCnt = 0;
	FRead(iTileInfoCnt, _pFile);
	m_vecTileInfo.clear();
	m_vecTileInfo.resize(iTileInfoCnt);
	for (UINT i = 0; i < iTileInfoCnt; ++i)
		FRead(m_vecTileInfo[i], _pFile);

	FRead(m_iTileXCnt, _pFile);
	FRead(m_iTileYCnt, _pFile);
	FRead(m_iAtlasTileXCnt, _pFile);
	FRead(m_iAtlasTileYCnt, _pFile);

	_InsertTileInfoToBuffer();
	return true;
}