#pragma once
// Component, Quad - Unreal Engine Landscape�� ������ ��� 

#include "CRenderer.h"
#include "CRaycastShader.h"
#include "CHeightMapShader.h"
#include "CWeightMapShader.h"

struct TWeight4 {
	float arrWeight[4];
};
struct TWeight8 {
	float arrWeight[8];
};

enum class E_TerrainMode {
	HeightMap,
	Splat,
	NONE,
};

class CTerrain : public CRenderer {
private:
	SharedPtr<CMesh> m_pMesh;
	SharedPtr<CMaterial> m_pMtrl;

	SharedPtr<CTexture> m_pHeightMapTex;
	SharedPtr<CTexture> m_pWeightMapTex;
	SharedPtr<CTexture> m_pBrushArrTex;

	SharedPtr<CRaycastShader> m_pCSRaycast;
	SharedPtr<CHeightMapShader> m_pCSHeightMap;
	SharedPtr<CWeightMapShader> m_pCSWeightMap;

	CStructuredBuffer* m_pCrossBuffer; // ���콺 ��ŷ�Ǵ� ������ �޴� ����

	CStructuredBuffer* m_pWeightMapBuffer;
	UINT m_iWeightWidth; // ����ġ ���� ���� ��ļ�
	UINT m_iWeightHeight; // ����ġ ���� ���� ��ļ�
	UINT m_iWeightIdx;	// ������ų ����ġ ����

	E_TerrainMode m_eTerrainMode; // ���� ����忡�� ���°�

	// Brush ũ�� (����)
	Vector2 m_vBrushScale;
	UINT m_iBrushIdx;

	// ���� ����
	UINT m_iQuadX;
	UINT m_iQuadZ;

	// ������Ʈ ����
	UINT m_iComponentX;
	UINT m_iComponentZ;

	// ���� ���� �� ����
	UINT m_iFaceX;
	UINT m_iFaceZ;

public:
	virtual void UpdateData() override {};
	virtual void FinalUpdate() override;
	virtual void Render() override;

	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	void Create();

public:
	SharedPtr<CMesh> GetMesh() { return m_pMesh; }
	SharedPtr<CMaterial> GetMaterial() { return m_pMtrl; }
	void SetHeightMapTex(SharedPtr<CTexture> _pTex);
	void SetWeightMapTex(SharedPtr<CTexture> _pTex);
	SharedPtr<CTexture> GetHeightMapTex() { return m_pHeightMapTex; }
	SharedPtr<CTexture> GetWeightMapTex() { return m_pWeightMapTex; }
	void SetQuard(UINT _iX, UINT _iZ) { m_iQuadX = _iX, m_iQuadZ = _iZ; }
	UINT GetQuadX() { return m_iQuadX; }
	UINT GetQuadZ() { return m_iQuadZ; }
	void SetComponent(UINT _iX, UINT _iZ) { m_iComponentX = _iX, m_iComponentZ = _iZ; }
	UINT GetComponentX() { return m_iComponentX; }
	UINT GetComponentZ() { return m_iComponentZ; }
	UINT GetFaceX() { return m_iFaceX; }
	UINT GetFaceZ() { return m_iFaceZ; }
	SharedPtr<CTexture> GetBrushTex() { return m_pBrushArrTex; }
	void ChangeMode(E_TerrainMode _eMode) { m_eTerrainMode = _eMode; }
	E_TerrainMode GetTerrainMode() { return m_eTerrainMode; }
	void SetBrushScale(const Vector2& _vScale) { m_vBrushScale = _vScale; }
	const Vector2& GetBrushScale() { return m_vBrushScale; }

private:
	void _Raycasting();

public:
	CLONE(CTerrain);
	CTerrain();
	CTerrain(const CTerrain& _origin);
	virtual ~CTerrain() override;
};