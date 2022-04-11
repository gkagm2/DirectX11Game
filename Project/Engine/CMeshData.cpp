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
#include "CPerformanceMonitor.h"

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
	if (false == m_pMesh->IsAnimMesh())
		return pNewObj;

	CAnimator3D* pAnimator = new CAnimator3D;
	pNewObj->AddComponent(pAnimator);

	pAnimator->SetBones(m_pMesh->GetBones());
	pAnimator->SetAnimClip(m_pMesh->GetAnimClip());

	return pNewObj;
}

CMeshData* CMeshData::LoadFromFBX(const wstring& _strPath)
{
#ifdef _DEBUG
	TCHAR szStr[128] = {};
	_stprintf_s(szStr, 128, _T("Load FBX (CMeshData class) [%s]\n"), _strPath.c_str());
#endif
	PM_START(szStr);
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

	PM_END();

	return pMeshData;
}

bool CMeshData::Save(const tstring& _strRelativePath)
{
	tstring strFilePath = CPathManager::GetInstance()->GetContentPath();
	strFilePath += _strRelativePath;
	SetRelativePath(_strRelativePath);

	FILE* pFile = nullptr;
	errno_t err = _tfopen_s(&pFile, strFilePath.c_str(), _T("wb"));
	if (err) {
		assert(nullptr);
		return false;
	}

	// Mesh �� ���Ϸ� ����
	m_pMesh->Save(m_pMesh->GetRelativePath());

	// Mesh Key ����	
	// Mesh Data ����
	SaveResourceToFile(m_pMesh, pFile);


	// material ���� ����
	UINT iMtrlCount = (UINT)m_vecMtrl.size();
	FWrite(iMtrlCount, pFile);


	UINT i = 0;
	for (; i < iMtrlCount; ++i) {
		if (nullptr == m_vecMtrl[i])
			continue;

		// Material �� ���Ϸ� ����
		m_vecMtrl[i]->Save(m_vecMtrl[i]->GetRelativePath());

		// Material �ε���, Key, Path ����
		FWrite(i, pFile);
		SaveResourceToFile(m_vecMtrl[i], pFile);
	}

	i = -1; // ���� ��
	FWrite(i, pFile);

	fclose(pFile);

	return true;
}

int CMeshData::Load(const tstring& _strFilePath)
{
	FILE* pFile = NULL;
	
	errno_t err;
	err = _tfopen_s(&pFile, _strFilePath.c_str(), _T("rb"));
	if (err) {
		assert(pFile);
		return E_FAIL;
	}

	// Mesh Load
	LoadResourceFromFile(m_pMesh, pFile);
	assert(m_pMesh.Get());

	// material ���� �б�
	UINT iMtrlCount = 0;
	FRead(iMtrlCount, pFile);

	m_vecMtrl.resize(iMtrlCount);

	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		UINT idx = -1;
		FRead(idx, pFile);
		if (idx == -1)
			break;

		wstring strKey, strPath;

		SharedPtr<CMaterial> pMtrl;
		LoadResourceFromFile(pMtrl, pFile);

		m_vecMtrl[i] = pMtrl;
	}

	fclose(pFile);

	return S_OK;
}