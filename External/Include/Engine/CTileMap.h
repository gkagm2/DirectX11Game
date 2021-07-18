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

	SharedPtr<CTexture> m_pAtlasTexture; // 아틀라스 텍스쳐

	// Atlas Texture에서 Tile 하나의 사이즈
	Vector2 m_vTexTileSize;
	Vector2 m_vTexSize; // 아틀라스 텍스쳐 사이즈

	int m_iDefaultElementCountCol; // 기본으로 세팅 할 타일맵의 Column 크기
	int m_iDefaultElementCountRow; // 기본으로 세팅 할 타일맵의 Row 크기

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
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CTileMap);
	CTileMap();
	virtual ~CTileMap() override;
};