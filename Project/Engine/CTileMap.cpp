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
	m_vTexTileSize{},
	m_vTexSize{},
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
	m_pMaterial->SetData(E_ShaderParam::Vector2_0, &vLT); // �������� LeftTop

	Vector2 vSizeUV;
	vSizeUV.x = 1.f / m_iAtlasTileCol;
	vSizeUV.y = 1.f / m_iAtlasTileRow;
	m_pMaterial->SetData(E_ShaderParam::Vector2_1, &vSizeUV); // Tile �ϳ��� ������


	// ������ Ÿ���� ���� ����
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
	m_pTileMapBuffer->SetData(m_vecTiles.data(), (UINT)m_vecTiles.size());
	m_pTileMapBuffer->UpdateData(REGISTER_NUM_TileMapBuffer);
}

void CTileMap::SetTileAtlas(SharedPtr<CTexture> _pAtlasTexture, const Vector2& _iTexTileSize)
{
	m_vTexTileSize = _iTexTileSize;

	m_pMaterial->SetData(E_ShaderParam::Texture_0, _pAtlasTexture.Get());

	m_pAtlasTexture = _pAtlasTexture;
	m_vTexSize = _pAtlasTexture.Get()->GetDimension();
	m_iAtlasTileCol = (int)(m_vTexSize.x / _iTexTileSize.x);
	m_iAtlasTileRow = (int)(m_vTexSize.y / _iTexTileSize.y);

	m_vecTiles.clear();
	for (int y = 0; y < m_iAtlasTileRow; ++y) {
		for (int x = 0; x < m_iAtlasTileCol; ++x) {
			TTileInfo tTileInfo = {};
			tTileInfo.vLeftTop = Vector2(x * _iTexTileSize.x, y * _iTexTileSize.y);
			tTileInfo.vLeftTopUV = Vector2(x * _iTexTileSize.x / m_vTexSize.x, y * _iTexTileSize.y / m_vTexSize.y);
			tTileInfo.vRightBottom = Vector2((x * _iTexTileSize.x + _iTexTileSize.x), (y * _iTexTileSize.y + _iTexTileSize.y));
			tTileInfo.vRightBottomUV = Vector2((x * _iTexTileSize.x + _iTexTileSize.x) / m_vTexSize.x, (y * _iTexTileSize.y + _iTexTileSize.y) / m_vTexSize.y);
			tTileInfo.vTileSize = Vector2(m_vTexTileSize);
			tTileInfo.vTileSizeUV = Vector2(m_vTexTileSize / m_vTexSize);
			m_vecTiles.push_back(tTileInfo);
		}
	}

	m_pTileMapBuffer->Create(E_StructuredBufferType::ReadOnly, sizeof(TTileInfo), (UINT)m_vecTiles.size());
}

void CTileMap::CreateTile(int _iCol, int _iRow)
{
	m_iTileCol = _iCol;
	m_iTileRow = _iRow;
}

bool CTileMap::SaveToScene(FILE* _pFile)
{
	SaveResourceToFile(m_pMesh, _pFile);
	//SaveResourceToFile(m_pMaterial, _pFile);

	SaveResourceToFile(m_pAtlasTexture, _pFile);
	FWrite(m_vTexTileSize, _pFile);

	FWrite(m_iTileCol, _pFile);
	FWrite(m_iTileRow, _pFile);

	return true;
}

bool CTileMap::LoadFromScene(FILE* _pFile)
{
	LoadResourceFromFile(m_pMesh, _pFile);
	//LoadResourceFromFile(m_pMaterial, _pFile);

	LoadResourceFromFile(m_pAtlasTexture, _pFile);
	FRead(m_vTexTileSize, _pFile);

	SetTileAtlas(m_pAtlasTexture, m_vTexTileSize);

	FRead(m_iTileCol, _pFile);
	FRead(m_iTileRow, _pFile);

	return true;
}