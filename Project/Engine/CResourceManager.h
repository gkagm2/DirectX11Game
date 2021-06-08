#pragma once
#include "CMesh.h"
#include "CGraphicsShader.h"
#include "CComputeShader.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CPathManager.h"

class CResourceManager : public CSingleton<CResourceManager>
{
	SINGLETON(CResourceManager)
private:
	unordered_map<tstring, CResource*> m_umapResource[(UINT)E_ResourceType::End];

public:
	void Init();

public:
	void CreateDefaultMesh();
	void CreateDefaultCubeMesh3D();
	void CreateDefaultShader();
	void CreateDefaultMaterial();

public:
	// TODO : ���߿� ���̴� �ڵ��� �Լ��̸�..etc �� �о���� �� ��� �� ����.
	template<typename T>
	SharedPtr<T> Load(const tstring& _strKey, const tstring& _strRelativePath);

	template<typename T>
	void AddRes(const tstring& _strKey, T* _pRes);

	template<typename T>
	SharedPtr<T> FindRes(const tstring& _strKey);
};

// ���ҽ� �߰� �� Ÿ�� ����
template<typename T>
E_ResourceType GetResourceType() {
	const type_info& info = typeid(T);

	const type_info& mtrl = typeid(CMaterial);
	const type_info& mesh = typeid(CMesh);
	const type_info& graphicsShader = typeid(CGraphicsShader);
	const type_info& computeShader= typeid(CComputeShader);
	const type_info& texture = typeid(CTexture);
	
	E_ResourceType eResourceType = E_ResourceType::End;

	if (&info == &mtrl)
		eResourceType = E_ResourceType::Material;
	else if (&info == &mesh)
		eResourceType = E_ResourceType::Mesh;
	else if (&info == &graphicsShader || &info == &computeShader)
		eResourceType = E_ResourceType::Shader;
	else if (&info == &texture)
		eResourceType = E_ResourceType::Texture;

	return eResourceType;
}

template<typename T>
inline SharedPtr<T> CResourceManager::Load(const tstring& _strKey, const tstring& _strRelativePath)
{
	E_ResourceType eType = GetResourceType<T>();

	T* pResource = FindRes<T>(_strKey).Get();
	if (nullptr != pResource) {
		MessageBox(nullptr, STR_MSG_FailDuplicateResourceKey, STR_MSG_FailedToLoadResource, MB_OK);
		assert(nullptr);
		return nullptr;
	}

	pResource = new T;
	tstring strFilePath = CPathManager::GetInstance()->GetContentPath() + _strRelativePath;

	if (FAILED(((CResource*)pResource)->Load(strFilePath))) {
		assert(nullptr);
		return nullptr;
	}

	pResource->SetKey(_strKey);
	pResource->SetRelativePath(_strRelativePath);
	AddRes<T>(_strKey, pResource);

	return pResource;
}

template<typename T>
inline void CResourceManager::AddRes(const tstring& _strKey, T* _pRes)
{
	E_ResourceType eResourceType = GetResourceType<T>();
	m_umapResource[(UINT)eResourceType].insert(make_pair(_strKey, _pRes));
}

template<typename T>
inline SharedPtr<T> CResourceManager::FindRes(const tstring& _strKey)
{
	E_ResourceType eResourceType = GetResourceType<T>();

	if (eResourceType == E_ResourceType::End) {
		assert(nullptr && _T("Resource�� Ÿ���� �������� ����"));
		return nullptr;
	}

	auto iter = m_umapResource[(UINT)eResourceType].find(_strKey);
	auto endIter = m_umapResource[(UINT)eResourceType].end();

	if (iter == endIter) {
		return nullptr;
	}
	T* pResource = dynamic_cast<T*>(iter->second);

	if (nullptr == pResource)
		assert(nullptr && _T("Resource�� ã�� ����."));

	return pResource;
}