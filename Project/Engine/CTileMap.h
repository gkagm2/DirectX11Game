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

	SharedPtr<CTexture> m_pAtlasTexture; // 아틀라스 텍스쳐

	unique_ptr<CStructuredBuffer> m_pTileMapBuffer;

	vector<TTileInfo> m_vecTileInfo; // 타일 정보

	// 생성할 Tile의 개수
	int m_iTileXCnt;
	int m_iTileYCnt;

	// 아틀라스 텍스쳐 Tile의 개수
	int m_iAtlasTileXPixelSize;
	int m_iAtlasTileYPixelSize;

	int m_iDefaultTileColCnt; // 기본으로 세팅 할 타일맵의 Column 크기
	int m_iDefaultTileRowCnt; // 기본으로 세팅 할 타일맵의 Row 크기

public:
	virtual void FinalUpdate() override;
	virtual void UpdateData() override;
	virtual void Render() override;

public:
	int GetCol() { return m_iTileXCnt; }
	int GetRow() { return m_iTileYCnt; }

	int GetAtlasTileXSize() { return m_iAtlasTileXPixelSize; }
	int GetAtlasTileYSize() { return m_iAtlasTileYPixelSize; }

	SharedPtr<CMesh> GetMesh() { return m_pMesh; }
	SharedPtr<CMaterial> GetMaterial() { return m_pMaterial; }
	void SetMesh(SharedPtr<CMesh> _pMesh) { m_pMesh = _pMesh; }
	void SetMaterial(SharedPtr<CMaterial>_pMaterial) { m_pMaterial = _pMaterial; }

	void SetTileAtlasTexture(SharedPtr<CTexture> _pAtlasTexture) { m_pAtlasTexture = _pAtlasTexture; }
	SharedPtr<CTexture> GetAtlasTexture() {	return m_pAtlasTexture;	}

	// 아틀라스 텍스쳐의 타일의 개수
	void SetAtlasTilePixelSize(UINT _iX, UINT _iY) {
		m_iAtlasTileXPixelSize= _iX;
		m_iAtlasTileYPixelSize = _iY; }

	vector<TTileInfo>& GetTilesInfo() { return m_vecTileInfo; }

	// 아틀라스의 타일 1개의 픽셀 사이즈
	int GetAtlasTileXPixelSize() { return m_iAtlasTileXPixelSize; }
	int GetAtlasTileYPixelSize() { return m_iAtlasTileYPixelSize; }
	Vector2 GetAtlasTilePixelSize() { return Vector2(GetAtlasTileXPixelSize(), GetAtlasTileYPixelSize()); }

	int GetAtlasTileXCnt() {
		return GetAtlasTexture()->GetResolution().x / m_iAtlasTileXPixelSize;
	}
	int GetAtlasTileYCnt() {
		return GetAtlasTexture()->GetResolution().y / m_iAtlasTileYPixelSize;
	}

private:
	void _InsertTileInfoToBuffer();

public:
	bool CreateTile(UINT _iCol, UINT _iRow, bool _bIsBlankInit = true);

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CTileMap);
	CTileMap();
	CTileMap(const CTileMap& _origin);
	virtual ~CTileMap() override;
};