#pragma once
class CLine
{
private:
	vector<VTX> m_vecVtx; // ¡§¡°
	vector<int> m_vecIdx; // ¿Œµ¶Ω∫

	vector<CMesh*> m_vecMesh; // Line Mesh
	SharedPtr<CMaterial>m_pMtrl;

	Vector4 m_vLineColor;

public:
	void Init();
	void AddLine(const Vector3& _vPos1, const Vector3& _vPos2);
	void AddBuffer(const Vector3& _vPos1, const Vector3& _vPos2);
	void Clear();

public:
	void UpdateData();

public:
	CLONE(CLine);
	CLine();
	virtual ~CLine();
};