#pragma once
#include "CRenderer.h"
#include "CMesh.h"
#include "CMaterial.h"

struct tMtrlSet
{
	SharedPtr<CMaterial>  pMtrl;        // ���� ��� �� ���׸���
	SharedPtr<CMaterial>  pSharedMtrl;  // ���� ���׸���
	SharedPtr<CMaterial>  pClonedMtrl;  // ���� ���׸����� ���纻    
};

class CMeshRenderer : public CRenderer
{
private:
	SharedPtr<CMesh>			m_pMesh;
	//SharedPtr<CMaterial>		m_pMtrl;
	vector<tMtrlSet>			m_vecMtrls;

	UINT m_iColor;

public:
	virtual void Render() override;

public:
	void SetMesh(SharedPtr<CMesh> _pMesh);
	void SetMaterial(SharedPtr<CMaterial> _pMtrl, UINT _iIdx);


	SharedPtr<CMesh> GetMesh() { return m_pMesh; }
	SharedPtr<CMaterial> GetCloneMaterial(UINT _iIDx);
	SharedPtr<CMaterial> GetClone_NoSave(UINT _iIdx);
	SharedPtr<CMaterial> GetSharedMaterial(UINT _iIdx);

	void SetColor(BYTE _r, BYTE _g, BYTE _b, BYTE _a) {
		m_iColor = COLOR_RGBA(_r, _g, _b, _a);
	}
	UINT GetColor() { return m_iColor; }

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;
public:
	CLONE(CMeshRenderer);
	CMeshRenderer();
	virtual ~CMeshRenderer() override;
};