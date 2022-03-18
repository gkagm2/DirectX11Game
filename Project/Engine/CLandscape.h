#pragma once
#include "CRenderer.h"
// Component, Quad - Unreal Engine Landscape�� ������ ��� 

class CLandscape : public CRenderer {
private:
	SharedPtr<CMesh> m_pMesh;
	SharedPtr<CMaterial> m_pMtrl;

	SharedPtr<CTexture> m_pTex;
	SharedPtr<CTexture> m_pWeightMapTex;

	// ���� ����
	UINT m_iQuadX;
	UINT m_iQuadZ;

	// ������Ʈ ����
	UINT m_iComponentX;
	UINT m_iComponentZ;
	
public:
	virtual void UpdateData() override {};
	virtual void FinalUpdate() override;
	virtual void Render() override;

	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	SharedPtr<CMesh> GetMesh() { return m_pMesh; }
	SharedPtr<CMaterial> GetMaterial() { return m_pMtrl; }

	void SetQuard(UINT _iX, UINT _iZ) { m_iQuadX = _iX, m_iQuadZ = _iZ; }
	void SetComponent(UINT _iX, UINT _iZ) { m_iComponentX = _iX, m_iComponentZ = _iZ; }
	void Create();

public:
	UINT GetQuadX() { return m_iQuadX; }
	UINT GetQuadZ() { return m_iQuadZ; }
	UINT GetComponentX() { return m_iComponentX; }
	UINT GetComponentZ() { return m_iComponentZ; }

public:
	CLONE(CLandscape);
	CLandscape();
	virtual ~CLandscape() override;
};