#pragma once
#include "CMesh.h"
#include "CGraphicsShader.h"
#include "CComputeShader.h"

class CResourceManager : public CSingleton<CResourceManager>
{
	SINGLETON(CResourceManager)
private:
	unordered_map<tstring, CResource*> m_umapResource[(UINT)E_ResourceType::END];

public:
	void Init();

public:
	void CreateDefaultMesh();
	void CreateDefaultShader();

public:
	// TODO : ���߿� ���̴� �ڵ��� �Լ��̸�..etc �� �о���� �� ��� �� ����.
	template<typename T>
	CResource* Load(const tstring& _strKey, const tstring& _strRelativePath);

	template<typename T>
	void AddResource(const tstring& _strKey, T* _pRes);

	template<typename T>
	CResource* FindRes(const tstring& _strKey);

};

// ���ҽ� �߰� �� Ÿ�� ����
template<typename T>
E_ResourceType GetResourceType() {
	const type_info& info = typeid(T);

	const type_info& mesh = typeid(CMesh);
	const type_info& graphicsShader = typeid(CGraphicsShader);
	const type_info& computeShader= typeid(CComputeShader);
	
	E_ResourceType eResourceType = E_ResourceType::END;

	if (&info == &mesh)
		eResourceType = E_ResourceType::MESH;
	else if (&info == &graphicsShader || &info == &computeShader)
		eResourceType = E_ResourceType::SHADER;

	return eResourceType;
}

template<typename T>
inline CResource* CResourceManager::Load(const tstring& _strKey, const tstring& _strRelativePath)
{
	return nullptr;
}

template<typename T>
inline void CResourceManager::AddResource(const tstring& _strKey, T* _pRes)
{
	E_ResourceType eResourceType = GetResourceType<T>();
	m_umapResource[(UINT)eResourceType].insert(make_pair(_strKey, _pRes));
}

template<typename T>
inline CResource* CResourceManager::FindRes(const tstring& _strKey)
{
	E_ResourceType eResourceType = GetResourceType<T>();

	if (eResourceType == E_ResourceType::END) {
		assert(nullptr || _T("Resource�� Ÿ���� �������� ����"));
		return nullptr;
	}

	auto iter = m_umapResource[(UINT)eResourceType].find(_strKey);
	auto endIter = m_umapResource[(UINT)eResourceType].end();

	if (iter == endIter) {
		assert(nullptr || _T("Resource�� ã�� ����."));
		return nullptr;
	}
	return iter->second;
}