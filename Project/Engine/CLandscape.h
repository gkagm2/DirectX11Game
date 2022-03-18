#pragma once
#include "CRenderer.h"
// Component, Quad - Unreal Engine Landscape와 개념이 비슷 

class CLandscape : public CRenderer {
private:
	SharedPtr<CMesh> m_pMesh;
	SharedPtr<CMaterial> m_pMtrl;

	SharedPtr<CTexture> m_pTex;
	SharedPtr<CTexture> m_pWeightMapTex;

	// 쿼드 개수
	UINT m_iQuadX;
	UINT m_iQuadZ;

	// 컴포넌트 개수
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
	CLONE(CLandscape);
	CLandscape();
	virtual ~CLandscape() override;
};