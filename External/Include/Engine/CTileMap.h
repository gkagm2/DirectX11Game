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
	SharedPtr<CTexture> m_pAtlasTexture; // ��Ʋ�� �ؽ���

	unique_ptr<CStructuredBuffer> m_pTileMapBuffer;

	vector<TTileInfo> m_vecTileInfo; // Ÿ�� ����

	// ������ Tile�� ����
	int m_iTileXCnt;
	int m_iTileYCnt;

	// ��Ʋ�� �ؽ��� Tile�� ����
	int m_iAtlasTileXCnt;
	int m_iAtlasTileYCnt;

	int m_iDefaultTileColCnt; // �⺻���� ���� �� Ÿ�ϸ��� Column ũ��
	int m_iDefaultTileRowCnt; // �⺻���� ���� �� Ÿ�ϸ��� Row ũ��

public:
	virtual void FinalUpdate() override;
	virtual void UpdateData() override;
	virtual void Render() override;

public:
	void SetTileFaceSize(int _iCol, int _iRow);

	int GetCol() { return m_iTileXCnt; }
	int GetRow() { return m_iTileYCnt; }

	SharedPtr<CMesh> GetMesh() { return m_pMesh; }
	SharedPtr<CMaterial> GetMaterial() { return m_pMaterial; }
	void SetMesh(SharedPtr<CMesh> _pMesh) { m_pMesh = _pMesh; }
	void SetMaterial(SharedPtr<CMaterial>_pMaterial) { m_pMaterial = _pMaterial; }

	void SetTileAtlas(SharedPtr<CTexture> _pAtlasTexture);

	SharedPtr<CTexture> GetAtlasTexture() {	return m_pAtlasTexture;	}

private:
	void _InsertTileInfoToBuffer();

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CTileMap);
	CTileMap();
	CTileMap(const CTileMap& _origin);
	virtual ~CTileMap() override;
};