#pragma once

class CGameObject;
class CGrid
{
private:
	CGameObject* pTargetObj;
private:
	int m_iHeight;
	int m_iWidth;

	int m_iVtxCnt;
	int m_iIdxCnt;

	vector<VTX> m_vecVtx; // ¡§¡°
	vector<int> m_vecIdx; // ¿Œµ¶Ω∫

	vector< SharedPtr<CMesh>> m_vecMesh;
	SharedPtr<CMesh> m_pMesh;
	SharedPtr<CMaterial> m_pMtrl;

public:
	void Init(int ,int);
	void InitBuffer();
	
public:
	void UpdateData();

public:
	CGrid(CGameObject* _pTargetObj);
	virtual ~CGrid();
};