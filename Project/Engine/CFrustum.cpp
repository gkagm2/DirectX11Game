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

// NDC �ȿ� �ִ� ��ǥ��.
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
	// ���������� �����ϰ� �ִ� ī�޶��� ���� ��ġ, ȸ�����¸� �����ͼ�
	// ī�޶��� �þ� ������ ����ü(6���� ���) �� ǥ��
	Matrix matViewInv = m_pOwner->GetViewInverseMatrix();
	Matrix matProjInv = m_pOwner->GetProjectionInverseMatrix();
	Matrix matInv = matProjInv * matViewInv;

	// �������� ����(NDC) �� ������ ��Ÿ���� 8���� ������ �ٽ� view, world ������ �ǵ��� ����
	// �׷��� ���忡���� ī�޶� �þ� ���� ��ġ�� ���� �� ����
	Vector3 vWorldPos[8] = {};
	for (int i = 0; i < 8; ++i)
		vWorldPos[i] = XMVector3TransformCoord(g_arrProjPos[i], matInv);

	// ī�޶��� �þ� ������ ��Ÿ�� 6���� ����� �����Ѵ�.
	m_arrFace[(UINT)E_FaceType::Near] = XMPlaneFromPoints(vWorldPos[0], vWorldPos[1], vWorldPos[2]);
	m_arrFace[(UINT)E_FaceType::Far] = XMPlaneFromPoints(vWorldPos[5], vWorldPos[4], vWorldPos[7]);

	m_arrFace[(UINT)E_FaceType::Up] = XMPlaneFromPoints(vWorldPos[4], vWorldPos[5], vWorldPos[1]);
	m_arrFace[(UINT)E_FaceType::Down] = XMPlaneFromPoints(vWorldPos[6], vWorldPos[7], vWorldPos[2]);

	m_arrFace[(UINT)E_FaceType::Left] = XMPlaneFromPoints(vWorldPos[4], vWorldPos[0], vWorldPos[7]);
	m_arrFace[(UINT)E_FaceType::Right] = XMPlaneFromPoints(vWorldPos[5], vWorldPos[6], vWorldPos[1]);
}


bool CFrustum::CheckPoint(const Vector3& _vPos)
{
	// �Է����� ���� ������ ī�޶��� �þ߹��� 6���� �� ���ο� �������� Ȯ���ϱ� ����
	// 6���� ���� 6���� ���� �׽�Ʈ�� �����ؼ� ���� ���������� �߸� �ش� ���� ���γ��ο� �ִ��� Ȯ��.
	for (UINT i = 0; i < UINT(E_FaceType::End); ++i) {
		// N �� P = d (�� P �� ������� �ִ�)
		// N �� P < d (�� P �� ����� Normal �� ����Ű�� ���ʿ� �ִ�)
		// N �� P > d (�� P �� ����� Normal �� ����Ű�� �������� �ۿ� �ִ�)
		Vector3 vNormal = m_arrFace[(UINT)i];
		if (vNormal.Dot(_vPos) + m_arrFace[(UINT)i].w > 0)
			return false;
	}
	return true;
}

bool CFrustum::CheckSphere(const Vector3& _vCenter, float _fRadius)
{
	// �Է����� ���� ������
	// ī�޶��� �þ߹��� 6���� �� ���ο� �������� Ȯ���ϱ� ����
	// 6���� ���� 6���� ���� �׽�Ʈ�� �����ؼ� ���� ���������� �߸� �ش� ���� ����ü ���ο� �־���.
	for (UINT i = 0; i < UINT(E_FaceType::End); ++i) {
		// N �� P = d (�� P �� ������� �ִ�)
		// N �� P < d (�� P �� ����� Normal �� ����Ű�� ���ʿ� �ִ�)
		// N �� P > d (�� P �� ����� Normal �� ����Ű�� �������� �ۿ� �ִ�)
		Vector3 vNormal = m_arrFace[(UINT)i];
		if (vNormal.Dot(_vCenter) + m_arrFace[(UINT)i].w > _fRadius)
			return false;
	}
	return true;
}