#include "pch.h"
#include "COrthogonalLineScript.h"

COrthogonalLineScript::COrthogonalLineScript() :
	CScript(-1),
	m_pCloneMtrl{ nullptr },
	m_vXDir{Vector3::Right},
	m_vYDir{Vector3::Up},
	m_vZDir{Vector3::Front},
	m_fLength{3.f}
{
}

COrthogonalLineScript::~COrthogonalLineScript()
{
}

void COrthogonalLineScript::Start()
{
	m_pCloneMtrl = MeshRenderer()->GetCloneMaterial(0);

	// 라인 초기화
	m_vecXLine.push_back(Vector3(0.f, 0.f, 0.f));
	m_vecXLine.push_back(Vector3(m_fLength, 0.f, 0.f));
	AddLineVtx(m_vecXLine, Vector4(1.0f, 0.f, 0.f, 1.0f));

	m_vecYLine.push_back(Vector3(0.f, 0.f, 0.f));
	m_vecYLine.push_back(Vector3(0.f, m_fLength, 0.f));
	AddLineVtx(m_vecYLine, Vector4(0.f, 1.f, 0.f, 1.f));

	m_vecZLine.push_back(Vector3(0.f, 0.f, 0.f));
	m_vecZLine.push_back(Vector3(0.f, 0.f, m_fLength));
	AddLineVtx(m_vecZLine, Vector4(0.f, 0.f, 1.f, 1.f));

	SharedPtr<CMesh> pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_LineStripMesh);
	SharedPtr<CMaterial> pMaterial = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_LineStripMtrl);
	assert(pMesh.Get());
	assert(pMaterial.Get());

	m_pCloneMtrl = pMaterial;
}

void COrthogonalLineScript::Update()
{
	// TODO (Jang) : Line Strip Rendering
	// Draw Line
	//m_pCloneMtrl->UpdateData();

}

void COrthogonalLineScript::AddLineVtx(const vector<Vector3>& _vecLineVertex, const Vector4& _vColor)
{
	VTX vtx;
	vector<VTX> vecVtx;
	for (UINT i = 0; i < _vecLineVertex.size(); ++i) {
		vtx.vPos = _vecLineVertex[i];
		vtx.vColor = _vColor;
		vecVtx.push_back(vtx);
	}
}