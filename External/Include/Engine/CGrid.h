#pragma once

class CGameObject;
class CGrid
{
private:
	CGameObject* m_pTargetObj;

private:
	vector<VTX> m_vecVtx; // ¡§¡°
	vector<int> m_vecIdx; // ¿Œµ¶Ω∫

	vector<SharedPtr<CMesh>> m_vecMesh; // Line Mesh
	SharedPtr<CMesh> m_pMesh;
	SharedPtr<CMaterial> m_pMtrl;

	Vector4 m_vGridColor;

public:
	void Init();
	void AddBuffer(const Vector3& _vPos1, const Vector3& _vPos2);
	void Clear();
	
public:
	void UpdateData();
	void SetTargetObject(CGameObject* _pTargetObj) { m_pTargetObj = _pTargetObj; }

public:
	CLONE(CGrid);
	CGrid(CGameObject* _pTargetObj);
	CGrid(const CGrid& _Owner);
	virtual ~CGrid();
};