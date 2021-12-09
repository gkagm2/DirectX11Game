#pragma once
#include "CComponent.h"
#include "CMesh.h"
#include "CMaterial.h"

class CStructuredBuffer;
class CTileMapGrid;
class CTileMap : public CComponent
{
private:
	SharedPtr<CMesh> m_pMesh;
	SharedPtr<CMaterial> m_pMaterial;

	SharedPtr<CTexture> m_pAtlasTexture; // ��Ʋ�� �ؽ���

	unique_ptr<CStructuredBuffer> m_pTileMapBuffer;

	vector<TTileInfo> m_vecTileInfo; // Ÿ�� ����

	bool m_bFrameVisible;

	// ������ Tile�� ����
	int m_iTileXCnt;
	int m_iTileYCnt;

	// ��Ʋ�� �ؽ��� Tile�� ����
	int m_iAtlasTileXPixelSize;
	int m_iAtlasTileYPixelSize;

	int m_iDefaultTileColCnt; // �⺻���� ���� �� Ÿ�ϸ��� Column ũ��
	int m_iDefaultTileRowCnt; // �⺻���� ���� �� Ÿ�ϸ��� Row ũ��

private:
	// Grid frame
	CTileMapGrid* m_pGrid;
	SharedPtr<CMesh> m_pGridMesh;
	SharedPtr<CMaterial> m_pGridMtrl;

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

	// ��Ʋ�� �ؽ����� Ÿ���� ����
	void SetAtlasTilePixelSize(UINT _iX, UINT _iY) {
		m_iAtlasTileXPixelSize= _iX;
		m_iAtlasTileYPixelSize = _iY; }

	vector<TTileInfo>& GetTilesInfo() { return m_vecTileInfo; }

	// ��Ʋ���� Ÿ�� 1���� �ȼ� ������
	int GetAtlasTileXPixelSize() { return m_iAtlasTileXPixelSize; }
	int GetAtlasTileYPixelSize() { return m_iAtlasTileYPixelSize; }
	Vector2 GetAtlasTilePixelSize() { return Vector2(GetAtlasTileXPixelSize(), GetAtlasTileYPixelSize()); }

	int GetAtlasTileXCnt() {
		return (int)(GetAtlasTexture()->GetResolution().x / m_iAtlasTileXPixelSize);
	}
	int GetAtlasTileYCnt() {
		return int(GetAtlasTexture()->GetResolution().y / m_iAtlasTileYPixelSize);
	}

	void SetFrameVisible(bool _bVisible) { m_bFrameVisible = _bVisible; }
	bool GetFrameVisible() { return m_bFrameVisible; }

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