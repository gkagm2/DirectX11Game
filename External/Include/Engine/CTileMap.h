#pragma once
#include "CComponent.h"
#include "CMesh.h"
#include "CMaterial.h"

class CStructuredBuffer;
class CTileMap : public CComponent
{
private:
	SharedPtr<CMesh> m_pMesh;
	SharedPtr<CMaterial> m_pMaterial;

	CStructuredBuffer* m_pTileMapBuffer;
	vector<TTileInfo> m_vecTiles;

	// 생성할 Tile의 개수
	int m_iTileCol;
	int m_iTileRow;

	// 아틀라스 텍스쳐 Tile의 개수
	int m_iAtlasTileCol;
	int m_iAtlasTileRow;

	// Atlas Texture에서 Tile 하나의 사이즈
	Vector2 m_vTexTileSize;
	Vector2 m_vTexSize; // 아틀라스 텍스쳐 사이즈

public:
	virtual void FinalUpdate() override;
	virtual void Render() override;

private:
	void _RenderInit();

public:
	// _iTexTileSize : Atlas Texture에서 Tile 하나의 사이즈
	void SetTileAtlas(SharedPtr<CTexture> _pAtlasTexture, const Vector2& _iTexTileSize);

	void CreateTile(int _iCol, int _iRow);

public:
	CLONE(CTileMap);
	CTileMap();
	virtual ~CTileMap() override;
};