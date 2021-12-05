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
	int m_iAtlasTileXCnt;
	int m_iAtlasTileYCnt;

	int m_iDefaultTileColCnt; // 기본으로 세팅 할 타일맵의 Column 크기
	int m_iDefaultTileRowCnt; // 기본으로 세팅 할 타일맵의 Row 크기

public:
	virtual void FinalUpdate() override;
	virtual void UpdateData() override;
	virtual void Render() override;

public:
	int GetCol() { return m_iTileXCnt; }
	int GetRow() { return m_iTileYCnt; }

	int GetAtlasTileCol() { return m_iAtlasTileXCnt; }
	int GetAtlasTileRow() { return m_iAtlasTileYCnt; }

	SharedPtr<CMesh> GetMesh() { return m_pMesh; }
	SharedPtr<CMaterial> GetMaterial() { return m_pMaterial; }
	void SetMesh(SharedPtr<CMesh> _pMesh) { m_pMesh = _pMesh; }
	void SetMaterial(SharedPtr<CMaterial>_pMaterial) { m_pMaterial = _pMaterial; }

	void SetTileAtlasTexture(SharedPtr<CTexture> _pAtlasTexture) { m_pAtlasTexture = _pAtlasTexture; }
	SharedPtr<CTexture> GetAtlasTexture() {	return m_pAtlasTexture;	}

	// 아틀라스 텍스쳐의 타일의 개수
	void SetAtlasTileCnt(UINT _iCol, UINT _iRow) {
		m_iAtlasTileXCnt = _iCol;
		m_iAtlasTileYCnt = _iRow; }

	vector<TTileInfo>& GetTilesInfo() { return m_vecTileInfo; }
	


private:
	void _InsertTileInfoToBuffer();

public:
	bool CreateTile(UINT _iCol, UINT _iRow);

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CTileMap);
	CTileMap();
	CTileMap(const CTileMap& _origin);
	virtual ~CTileMap() override;
};