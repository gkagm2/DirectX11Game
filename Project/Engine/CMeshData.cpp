#include "pch.h"
#include "CMeshData.h"

#include "CDevice.h"
#include "CPathManager.h"
#include "CMesh.h"
#include "CResourceManager.h"
#include "CMaterial.h"
#include "CTransform.h"
#include "CMeshRenderer.h"
#include "CGameObject.h"
#include "CAnimator3D.h"

CMeshData::CMeshData()
	: CResource(E_ResourceType::MeshData)
{
}

CMeshData::~CMeshData()
{
}


CGameObject* CMeshData::Instantiate()
{
	// Mesh
	// Material

	CGameObject* pNewObj = new CGameObject;
	pNewObj->AddComponent(new CTransform);
	pNewObj->AddComponent(new CMeshRenderer);

	pNewObj->MeshRenderer()->SetMesh(m_pMesh);

	for (UINT i = 0; i < m_vecMtrl.size(); ++i)
		pNewObj->MeshRenderer()->SetMaterial(m_vecMtrl[i], i);

	// Animation ��Ʈ �߰�
	CAnimator3D* pAnimator = new CAnimator3D;
	pNewObj->AddComponent(pAnimator);

	pAnimator->SetBones(m_pMesh->GetBones());
	pAnimator->SetAnimClip(m_pMesh->GetAnimClip());

	return pNewObj;
}

CMeshData* CMeshData::LoadFromFBX(const wstring& _strPath)
{
	wstring strFullPath = CPathManager::GetInstance()->GetContentPath();
	strFullPath += _strPath;

	CFBXLoader loader;
	loader.init();
	loader.LoadFbx(strFullPath);

	// �޽� ��������
	CMesh* pMesh = nullptr;
	pMesh = CMesh::CreateFromContainer(loader);

	// ResMgr �� �޽� ���
	wstring strMeshName = L"mesh\\";
	strMeshName += path(strFullPath).stem();
	strMeshName += L".mesh";

	pMesh->SetName(strMeshName);
	pMesh->SetRelativePath(strMeshName);

	CResourceManager::GetInstance()->AddRes<CMesh>(pMesh->GetName(), pMesh);

	vector<SharedPtr<CMaterial>> vecMtrl;

	// ���׸��� ��������
	for (UINT i = 0; i < loader.GetContainer(0).vecMtrl.size(); ++i)
	{
		// ����ó�� (material �̸��� �Է� �ȵǾ����� ���� �ִ�.)
		SharedPtr<CMaterial> pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(loader.GetContainer(0).vecMtrl[i].strMtrlName);
		vecMtrl.push_back(pMtrl);
	}

	CMeshData* pMeshData = new CMeshData;
	pMeshData->m_pMesh = pMesh;
	pMeshData->m_vecMtrl = vecMtrl;

	return pMeshData;
}