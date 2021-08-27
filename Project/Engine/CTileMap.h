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

	void SetEachFaceTextureSize(const Vector2& _vFaceTextureSize) {
		m_vTexTileSize = _vFaceTextureSize;
	}
	const Vector2& GetEachFaceTextureSize() { return m_vTexTileSize; }

	// atlas texture 설정
	// 생성 할 타일 사이즈를 설정한다.
	// 타일 하나의 픽셀 사이즈 설정
	// 타일로 분리한다.
	// 분리된 타일들의 정보를 가지고 있는 

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CTileMap);
	CTileMap();
	virtual ~CTileMap() override;
};