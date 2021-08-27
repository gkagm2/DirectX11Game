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

	// ������ Tile�� ����
	int m_iTileCol;
	int m_iTileRow;

	// ��Ʋ�� �ؽ��� Tile�� ����
	int m_iAtlasTileCol;
	int m_iAtlasTileRow;

	SharedPtr<CTexture> m_pAtlasTexture; // ��Ʋ�� �ؽ���

	// Atlas Texture���� Tile �ϳ��� ������
	Vector2 m_vTexTileSize;
	Vector2 m_vTexSize; // ��Ʋ�� �ؽ��� ������

	int m_iDefaultElementCountCol; // �⺻���� ���� �� Ÿ�ϸ��� Column ũ��
	int m_iDefaultElementCountRow; // �⺻���� ���� �� Ÿ�ϸ��� Row ũ��

public:
	virtual void FinalUpdate() override;
	virtual void Render() override;

private:
	void _RenderInit();

public:
	
	void SaperateTile(); // Ÿ�Ϸ� �и��ϴ�

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

	// atlas texture ����
	// ���� �� Ÿ�� ����� �����Ѵ�.
	// Ÿ�� �ϳ��� �ȼ� ������ ����
	// Ÿ�Ϸ� �и��Ѵ�.
	// �и��� Ÿ�ϵ��� ������ ������ �ִ� 

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CTileMap);
	CTileMap();
	virtual ~CTileMap() override;
};