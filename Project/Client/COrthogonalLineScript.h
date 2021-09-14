#pragma once
#include <Engine\CScript.h>
class COrthogonalLineScript : public CScript
{
private:
	SharedPtr<CMaterial> m_pCloneMtrl;

	Vector3 m_vXDir;
	Vector3 m_vYDir;
	Vector3 m_vZDir;
	float m_fLength;

private:
	vector<Vector3> m_vecXLine;
	vector<Vector3> m_vecYLine;
	vector<Vector3> m_vecZLine;

public:
	virtual void Start() override;
	virtual void Update() override;

public:
	void AddLine(vector<VTX>& _vecLineVertex);
	void AddLineVtx(const vector<Vector3>& _vecLineVertex, const Vector4& _vColor);

public:
	CLONE(COrthogonalLineScript);
	COrthogonalLineScript();
	virtual ~COrthogonalLineScript() override;
};