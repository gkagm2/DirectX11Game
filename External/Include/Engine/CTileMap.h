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

	int m_iDefaultElementCountCol; // 기본으로 세팅 할 타일맵의 Column 크기
	int m_iDefaultElementCountRow; // 기본으로 세팅 할 타일맵의 Row 크기

public:
	virtual void FinalUpdate() override;
	virtual void Render() override;

private:
	void _RenderInit();

public:
	
	void SaperateTile(); // 타일로 분리하다

	void SetTileFaceSize(int _iCol, int _iRow);

	int GetCol() { return m_iTileCol; }
	int GetRow() { return m_iTileRow; }

	SharedPtr<CMesh> GetMesh() { return m_pMesh; }
	SharedPtr<CMaterial> GetMaterial() { return m_pMaterial; }
	void SetMesh(SharedPtr<CMesh> _pMesh) { m_pMesh = _pMesh; }
	void SetMaterial(SharedPtr<CMaterial>_pMaterial) { m_pMaterial = _pMaterial; }

	void SetTileAtlas(SharedPtr<CTexture> _pAtlasTexture);

	SharedPtr<CTexture> GetAtlasTexture() {	return m_pAtlasTexture;	}

	void SetAtlasTileColRowSize(int _iCol, int _iRow) {
		m_iAtlasTileCol = _iCol;
		m_iAtlasTileRow = _iRow;
	}

public:


public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CTileMap);
	CTileMap();
	virtual ~CTileMap() override;
};