#include "pch.h"
#include "CTileMap.h"
#include "CTileMapGrid.h"
#include "CResourceManager.h"
#include "CTransform.h"
#include "CStructuredBuffer.h"
#include "CRenderManager.h"

CTileMap::CTileMap() :
	CComponent(E_ComponentType::TileMap),
	m_pTileMapBuffer(nullptr),
	m_iAtlasTileXPixelSize(0),
	m_iAtlasTileYPixelSize(0),
	m_iTileXCnt(2),
	m_iTileYCnt(2),
	m_bFrameVisible(false),
	m_iDefaultTileColCnt(2),
	m_iDefaultTileRowCnt(2),
	m_pGrid(nullptr)
{
	m_pMesh = CResourceManager::GetInstance()->LoadRes<CMesh>(STR_KEY_RectMesh);
	m_pMaterial = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_TileMapMtrl);


	m_vecTileInfo.resize(m_iDefaultTileColCnt * m_iDefaultTileRowCnt);
	for (int i = 0; i < m_vecTileInfo.size(); ++i)
		m_vecTileInfo[i].idx = 1;

	m_pTileMapBuffer = make_unique<CStructuredBuffer>();
	m_pTileMapBuffer->Create(E_StructuredBufferType::ReadOnly, sizeof(TTileInfo), (UINT)m_vecTileInfo.size(), true);

	if (!m_pGrid)
		m_pGrid = new CTileMapGrid(this);
	m_pGrid->Init();
}

CTileMap::CTileMap(const CTileMap& _origin) :
	CComponent(E_ComponentType::TileMap),
	m_pMesh(_origin.m_pMesh),
	m_pMaterial(_origin.m_pMaterial),
	m_pAtlasTexture(_origin.m_pAtlasTexture),
	m_pTileMapBuffer(nullptr),
	m_iAtlasTileXPixelSize(_origin.m_iAtlasTileXPixelSize),
	m_iAtlasTileYPixelSize(_origin.m_iAtlasTileYPixelSize),
	m_iTileXCnt(_origin.m_iTileXCnt),
	m_iTileYCnt(_origin.m_iTileYCnt),
	m_bFrameVisible(_origin.m_bFrameVisible),
	m_iDefaultTileColCnt(_origin.m_iDefaultTileColCnt),
	m_iDefaultTileRowCnt(_origin.m_iDefaultTileRowCnt)
{
	m_pTileMapBuffer = make_unique<CStructuredBuffer>();

	m_vecTileInfo.resize(m_iTileXCnt * m_iTileYCnt);
	for (int i = 0; i < m_vecTileInfo.size(); ++i)
		m_vecTileInfo[i].idx = _origin.m_vecTileInfo[i].idx;

	m_pTileMapBuffer->Create(E_StructuredBufferType::ReadOnly, sizeof(TTileInfo), (UINT)m_vecTileInfo.size(), m_vecTileInfo.data());

	if (!m_pGrid)
		m_pGrid = new CTileMapGrid(this);
	m_pGrid->Init();
}

CTileMap::~CTileMap()
{
	if (m_pGrid) {
		delete m_pGrid;
		m_pGrid = nullptr;
	}
}

void CTileMap::FinalUpdate()
{
}

void CTileMap::UpdateData()
{
	// -- 생성할 타일의 개수 세팅
	m_pMaterial->SetData(E_ShaderParam::Int_0, &m_iTileXCnt);
	m_pMaterial->SetData(E_ShaderParam::Int_1, &m_iTileYCnt);

	// -- 아틀라스 텍스쳐 세팅
	m_pMaterial->SetData(E_ShaderParam::Texture_0, m_pAtlasTexture.Get());

	// -- 아틀라스 텍스쳐의 사이즈
	Vector2 vAtlasResolution = {};
	if (nullptr != m_pAtlasTexture)
		vAtlasResolution = m_pAtlasTexture->GetResolution();
	m_pMaterial->SetData(E_ShaderParam::Vector2_0, &vAtlasResolution);

	// -- 아틀라스 텍스쳐에서 타일 하나의 UV 사이즈
	Vector2 vAtlasTileSize = {};
	try {
		vAtlasTileSize = Vector2(m_iAtlasTileXPixelSize, m_iAtlasTileYPixelSize);
	}
	catch (std::exception e) {
		vAtlasTileSize = Vector2(0.f, 0.f);
	}
	Vector2 vAtlasTileUVSize = {};
	try {
		vAtlasTileUVSize = vAtlasTileSize / vAtlasResolution;
	}
	catch (std::exception e) {
		vAtlasTileUVSize = Vector2{ 0.f, 0.f };
	}
	m_pMaterial->SetData(E_ShaderParam::Vector2_1, &vAtlasTileUVSize);

	Transform()->UpdateData();
	m_pMaterial->UpdateData();

	m_pTileMapBuffer->SetData(m_vecTileInfo.data(), sizeof(TTileInfo) * (UINT)m_vecTileInfo.size());
	m_pTileMapBuffer->UpdateData(REGISTER_NUM_TileMapBuffer, E_ShaderStage::Pixel);
}

void CTileMap::Render()
{
	UpdateData();
	m_pMesh->Render();

	if (!m_pGrid->DidInit())
		m_pGrid->Init();
	if (m_bFrameVisible && m_pGrid)
		m_pGrid->UpdateData();
	
	m_pMaterial->Clear();
}

void CTileMap::_InsertTileInfoToBuffer()
{
	m_pTileMapBuffer->Create(E_StructuredBufferType::ReadOnly, sizeof(TTileInfo), (UINT)m_vecTileInfo.size(), m_vecTileInfo.data());
}

bool CTileMap::CreateTile(UINT _iCol, UINT _iRow, bool _bIsBlankInit)
{
	if (0 == _iCol || 0 == _iRow)
		return false;

	m_iTileXCnt = _iCol;
	m_iTileYCnt = _iRow;

	UINT iTileCnt = m_iTileXCnt * m_iTileYCnt;
	m_vecTileInfo.clear();
	m_vecTileInfo.resize(iTileCnt);


	int ix = GetAtlasTileXCnt();
	int iy = GetAtlasTileYCnt();
	UINT iAtlasTileCnt = GetAtlasTileXCnt() * GetAtlasTileYCnt();

	if (_bIsBlankInit)
		for (size_t i = 0; i < m_vecTileInfo.size(); ++i)
			m_vecTileInfo[i].idx = -1;
	else
		for (size_t i = 0; i < m_vecTileInfo.size(); ++i)
			m_vecTileInfo[i].idx = i % (iAtlasTileCnt);

	_InsertTileInfoToBuffer();

	// 흠.. scale을 키워야되나..
	Transform()->SetLocalScale(Vector3(m_iTileXCnt, m_iTileYCnt, 1));

	if(!m_pGrid)
		m_pGrid = new CTileMapGrid(this); //Test
	m_pGrid->Init();

	return true;
}

bool CTileMap::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);
	SaveResourceToFile(m_pMesh, _pFile);
	SaveResourceToFile(m_pMaterial, _pFile);
	SaveResourceToFile(m_pAtlasTexture, _pFile);

#pragma region StructuedBuffer Save
	size_t iTileInfoCnt = m_vecTileInfo.size();
	FWrite(iTileInfoCnt, _pFile);
	for (size_t i = 0; i < iTileInfoCnt; ++i)
		FWrite(m_vecTileInfo[i], _pFile);
	// 다른 버전 (메모리 릭 이슈)
	/*UINT iElementCnt = m_pTileMapBuffer->GetElementCount();
	UINT iDataSize = m_pTileMapBuffer->GetElementSize() * iElementCnt;
	FWrite(iDataSize, _pFile);
	void* pData = malloc(iDataSize);
	m_pTileMapBuffer->GetData(pData, iElementCnt);
	fwrite(pData, iDataSize, 1, _pFile);*/
#pragma endregion

	FWrite(m_bFrameVisible, _pFile);
	FWrite(m_iTileXCnt, _pFile);
	FWrite(m_iTileYCnt, _pFile);
	FWrite(m_iAtlasTileXPixelSize, _pFile);
	FWrite(m_iAtlasTileYPixelSize, _pFile);

	bool bIsGridExist = nullptr == m_pGrid ? false : true;
	FWrite(bIsGridExist, _pFile);

	return true;
}

bool CTileMap::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);
	LoadResourceFromFile(m_pMesh, _pFile);
	LoadResourceFromFile(m_pMaterial, _pFile);
	LoadResourceFromFile(m_pAtlasTexture, _pFile);

#pragma region StructuedBuffer 불러오기
	size_t iTileInfoCnt = 0;
	FRead(iTileInfoCnt, _pFile);
	m_vecTileInfo.clear();
	m_vecTileInfo.resize(iTileInfoCnt);
	for (UINT i = 0; i < iTileInfoCnt; ++i)
		FRead(m_vecTileInfo[i], _pFile);
	// 다른 버전
	/*UINT iDataSize = 0;
	FRead(iDataSize, _pFile);
	void* pData = malloc(iDataSize);
	fread(pData, iDataSize, 1, _pFile);*/
#pragma endregion

	FRead(m_bFrameVisible, _pFile);
	FRead(m_iTileXCnt, _pFile);
	FRead(m_iTileYCnt, _pFile);
	FRead(m_iAtlasTileXPixelSize, _pFile);
	FRead(m_iAtlasTileYPixelSize, _pFile);

	_InsertTileInfoToBuffer();

	bool bIsGridExist = false;
	FRead(bIsGridExist, _pFile);
	if (bIsGridExist) {
		if (m_pGrid)
			delete m_pGrid;
		m_pGrid = new CTileMapGrid(this);
		m_pGrid->Init();
	}
	
	return true;
}