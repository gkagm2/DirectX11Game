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
	// _iTexTileSize : Atlas Texture���� Tile �ϳ��� ������
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