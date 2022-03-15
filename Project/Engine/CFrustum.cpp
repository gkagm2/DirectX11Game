#include "pch.h"
#include "CFrustum.h"
#include "CCamera.h"
#include "CResourceManager.h"
#include "CMesh.h"
#include "CGraphicsShader.h"
#include "CMaterial.h"

/*
   Cube
			   4--------5
			 / |        |
		0-------1  z=1  |
		|       |       |
		|  z=0 7|-------6
		|       |
		3-------2
	
*/

// NDC 안에 있는 좌표들.
const Vector3 CFrustum::g_arrProjPos[8] = {
	Vector3(-1.f, 1.f, 0.f),
	Vector3(1.f, 1.f, 0.f),
	Vector3(1.f, -1.f, 0.f),
	Vector3(-1.f, -1.f, 0.f),
	Vector3(-1.f, 1.f, 1.f),
	Vector3(1.f, 1.f, 1.f),
	Vector3(1.f, -1.f, 1.f),
	Vector3(-1.f, -1.f, 1.f),
};

CFrustum::CFrustum() :
	m_pOwner{ nullptr },
	m_arrFace{}
{
	m_pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_FrustumMesh);
	m_pMaterial =CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_Std3DFrameMtrl);
}

CFrustum::~CFrustum()
{
}

void CFrustum::UpdateData()
{
}

void CFrustum::Render()
{
	m_pMaterial->UpdateData();
	m_pMesh->Render();
	m_pMaterial->Clear();
}

void CFrustum::FinalUpdate()
{
	// 프러스텀을 소유하고 있는 카메라의 월드 위치, 회전상태를 가져와서
	// 카메라의 시야 범위를 절두체(6개의 평면) 로 표현
	Matrix matViewInv = m_pOwner->GetViewInverseMatrix();
	Matrix matProjInv = m_pOwner->GetProjectionInverseMatrix();
	Matrix matInv = matProjInv * matViewInv;

	// 투영공간 영역(NDC) 의 범위를 나타내는 8개의 정점을 다시 view, world 순으로 되돌려 보냄
	// 그러면 월드에서의 카메라 시야 범위 위치로 가게 될 것임
	Vector3 vWorldPos[8] = {};
	for (int i = 0; i < 8; ++i)
		vWorldPos[i] = XMVector3TransformCoord(g_arrProjPos[i], matInv);

	// 카메라의 시야 영역을 나타낼 6개의 평면을 정의한다.
	m_arrFace[(UINT)E_FaceType::Near] = XMPlaneFromPoints(vWorldPos[0], vWorldPos[1], vWorldPos[2]);
	m_arrFace[(UINT)E_FaceType::Far] = XMPlaneFromPoints(vWorldPos[5], vWorldPos[4], vWorldPos[7]);

	m_arrFace[(UINT)E_FaceType::Up] = XMPlaneFromPoints(vWorldPos[4], vWorldPos[5], vWorldPos[1]);
	m_arrFace[(UINT)E_FaceType::Down] = XMPlaneFromPoints(vWorldPos[6], vWorldPos[7], vWorldPos[2]);

	m_arrFace[(UINT)E_FaceType::Left] = XMPlaneFromPoints(vWorldPos[4], vWorldPos[0], vWorldPos[7]);
	m_arrFace[(UINT)E_FaceType::Right] = XMPlaneFromPoints(vWorldPos[5], vWorldPos[6], vWorldPos[1]);
}


bool CFrustum::CheckPoint(const Vector3& _vPos)
{
	// 입력으로 들어온 정점이 카메라의 시야범위 6개의 면 내부에 들어오는지 확인하기 위해
	// 6개의 평면과 6번의 내적 테스트를 진행해서 전부 내부판정이 뜨면 해당 점은 절두내부에 있는지 확인.
	for (UINT i = 0; i < UINT(E_FaceType::End); ++i) {
		// N ● P = d (점 P 는 평면위에 있다)
		// N ● P < d (점 P 는 평면의 Normal 이 가리키는 안쪽에 있다)
		// N ● P > d (점 P 는 평면의 Normal 이 가리키는 방향으로 밖에 있다)
		Vector3 vNormal = m_arrFace[(UINT)i];
		if (vNormal.Dot(_vPos) + m_arrFace[(UINT)i].w > 0)
			return false;
	}
	return true;
}

bool CFrustum::CheckSphere(const Vector3& _vCenter, float _fRadius)
{
	// 입력으로 들어온 정점이
	// 카메라의 시야범위 6개의 면 내부에 들어오는지 확인하기 위해
	// 6개의 평면과 6번의 내적 테스트를 진행해서 전부 내부판정이 뜨면 해당 점은 절두체 내부에 있었다.
	for (UINT i = 0; i < UINT(E_FaceType::End); ++i) {
		// N ● P = d (점 P 는 평면위에 있다)
		// N ● P < d (점 P 는 평면의 Normal 이 가리키는 안쪽에 있다)
		// N ● P > d (점 P 는 평면의 Normal 이 가리키는 방향으로 밖에 있다)
		Vector3 vNormal = m_arrFace[(UINT)i];
		if (vNormal.Dot(_vCenter) + m_arrFace[(UINT)i].w > _fRadius)
			return false;
	}
	return true;
}