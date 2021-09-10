#include "pch.h"
#include "CTileMap.h"
#include "CResourceManager.h"
#include "CTransform.h"
#include "CStructuredBuffer.h"
#include "CRenderManager.h"

CTileMap::CTileMap() :
	CComponent(E_ComponentType::TileMap),
	m_pTileMapBuffer(nullptr),
	m_iAtlasTileCol(0),
	m_iAtlasTileRow(0),
	m_iTileCol(4),
	m_iTileRow(4),
	m_iDefaultElementCountCol(10),
	m_iDefaultElementCountRow(10)
{
	m_pMesh = CResourceManager::GetInstance()->LoadRes<CMesh>(STR_KEY_RectMesh);
	m_pMaterial = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_TileMapMtrl);

	m_pTileMapBuffer = new CStructuredBuffer;
	const UINT iDefaultTileMapElementCnt = m_iDefaultElementCountCol * m_iDefaultElementCountRow;
	m_pTileMapBuffer->Create(E_StructuredBufferType::ReadOnly, sizeof(TTileInfo), iDefaultTileMapElementCnt);
}

CTileMap::~CTileMap()
{
	if (m_pTileMapBuffer)
		delete m_pTileMapBuffer;
}

void CTileMap::FinalUpdate()
{
	CRenderManager::GetInstance()->RegisterTileMap(this);
}

void CTileMap::Render()
{
	_RenderInit();

	Vector2 vLT = { 0.f / m_iAtlasTileCol, 1.f / m_iAtlasTileRow };
	m_pMaterial->SetData(E_ShaderParam::Vector2_0, &vLT); // 렌더링할 LeftTop

	Vector2 vSizeUV;
	vSizeUV.x = 1.f / m_iAtlasTileCol;
	vSizeUV.y = 1.f / m_iAtlasTileRow;
	m_pMaterial->SetData(E_ShaderParam::Vector2_1, &vSizeUV); // Tile 하나의 사이즈


	// 생성할 타일의 개수 세팅
	m_pMaterial->SetData(E_ShaderParam::Int_0, &m_iTileCol);
	m_pMaterial->SetData(E_ShaderParam::Int_1, &m_iTileRow);

	Transform()->UpdateData();
	m_pMesh->UpdateData();
	m_pMaterial->UpdateData();
	m_pMesh->Render();
	m_pMaterial->Clear();
}

void CTileMap::_RenderInit()
{
	m_pTileMapBuffer->SetData(m_vecTilesInfo.data(), (UINT)m_vecTilesInfo.size());
	m_pTileMapBuffer->UpdateData(REGISTER_NUM_TileMapBuffer);
}

void CTileMap::SaperateTile()
{
	Vector2 vAtlasTexSize = m_pAtlasTexture.Get()->GetDimension();
	Vector2 vEachAtlasTexSize;
	vEachAtlasTexSize.x = vAtlasTexSize.x / m_iAtlasTileCol;
	vEachAtlasTexSize.y = vAtlasTexSize.y / m_iAtlasTileRow;

	m_vecTilesInfo.clear();
	for (int y = 0; y < m_iAtlasTileRow; ++y) {
		for (int x = 0; x < m_iAtlasTileCol; ++x) {
			TTileInfo tTileInfo = {};
			tTileInfo.vLeftTop = Vector2(x * vEachAtlasTexSize.x, y * vEachAtlasTexSize.y);
			tTileInfo.vLeftTopUV = Vector2(x * vEachAtlasTexSize.x / vAtlasTexSize.x, y * vEachAtlasTexSize.y / vAtlasTexSize.y);
			tTileInfo.vRightBottom = Vector2((x * vEachAtlasTexSize.x + vEachAtlasTexSize.x), (y * vEachAtlasTexSize.y + vEachAtlasTexSize.y));
			tTileInfo.vRightBottomUV = Vector2((x * vEachAtlasTexSize.x + vEachAtlasTexSize.x) / vAtlasTexSize.x, (y * vEachAtlasTexSize.y + vEachAtlasTexSize.y) / vAtlasTexSize.y);
			tTileInfo.vTileSize = Vector2(vEachAtlasTexSize);
			tTileInfo.vTileSizeUV = Vector2(vEachAtlasTexSize / vAtlasTexSize);
			m_vecTilesInfo.push_back(tTileInfo);
		}
	}

	m_pTileMapBuffer->Create(E_StructuredBufferType::ReadOnly, sizeof(TTileInfo), (UINT)m_vecTilesInfo.size());
}

void CTileMap::SetTileFaceSize(int _iCol, int _iRow)
{
	m_iTileCol = _iCol;
	m_iTileRow = _iRow;
}

void CTileMap::SetTileAtlas(SharedPtr<CTexture> _pAtlasTexture)
{
	m_pAtlasTexture = _pAtlasTexture;
	m_pMaterial->SetData(E_ShaderParam::Texture_0, m_pAtlasTexture.Get());
}

void CTileMap::SetTile(UINT _iX, UINT _iY, UINT _iIdx)
{
	Vector2 vAtlasTexSize = m_pAtlasTexture.Get()->GetDimension();
	Vector2 vEachAtlasTexSize;
	vEachAtlasTexSize.x = vAtlasTexSize.x / m_iAtlasTileCol;
	vEachAtlasTexSize.y = vAtlasTexSize.y / m_iAtlasTileRow;


	TTileInfo tTileInfo = {};
	tTileInfo.vLeftTop = Vector2(_iX * vEachAtlasTexSize.x, _iY * vEachAtlasTexSize.y);
	tTileInfo.vLeftTopUV = Vector2(_iX * vEachAtlasTexSize.x / vAtlasTexSize.x, _iY * vEachAtlasTexSize.y / vAtlasTexSize.y);
	tTileInfo.vRightBottom = Vector2((_iX * vEachAtlasTexSize.x + vEachAtlasTexSize.x), (_iY * vEachAtlasTexSize.y + vEachAtlasTexSize.y));
	tTileInfo.vRightBottomUV = Vector2((_iX * vEachAtlasTexSize.x + vEachAtlasTexSize.x) / vAtlasTexSize.x, (_iY * vEachAtlasTexSize.y + vEachAtlasTexSize.y) / vAtlasTexSize.y);
	tTileInfo.vTileSize = Vector2(vEachAtlasTexSize);
	tTileInfo.vTileSizeUV = Vector2(vEachAtlasTexSize / vAtlasTexSize);

	m_vecTilesInfo.push_back(tTileInfo);
}

bool CTileMap::SaveToScene(FILE* _pFile)
{
	SaveResourceToFile(m_pMesh, _pFile);
	SaveResourceToFile(m_pMaterial, _pFile);

	SaveResourceToFile(m_pAtlasTexture, _pFile);

	FWrite(m_iTileCol, _pFile);
	FWrite(m_iTileRow, _pFile);
	FWrite(m_iAtlasTileCol, _pFile);
	FWrite(m_iAtlasTileRow, _pFile);

	return true;
}

bool CTileMap::LoadFromScene(FILE* _pFile)
{
	LoadResourceFromFile(m_pMesh, _pFile);
	LoadResourceFromFile(m_pMaterial, _pFile);

	LoadResourceFromFile(m_pAtlasTexture, _pFile);

	FRead(m_iTileCol, _pFile);
	FRead(m_iTileRow, _pFile);
	FRead(m_iAtlasTileCol, _pFile);
	FRead(m_iAtlasTileRow, _pFile);

	SetTileAtlas(m_pAtlasTexture);
	
	// 타일 분리
	SaperateTile();

	return true;
}