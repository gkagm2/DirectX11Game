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
	// TODO : 나중에 쉐이더 코드의 함수이름..etc 을 읽어들일 때 사용 할 것임.
	template<typename T>
	CResource* Load(const tstring& _strKey, const tstring& _strRelativePath);

	template<typename T>
	void AddResource(const tstring& _strKey, T* _pRes);

	template<typename T>
	T* FindRes(const tstring& _strKey);

};

// 리소스 추가 시 타입 지정
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
inline T* CResourceManager::FindRes(const tstring& _strKey)
{
	E_ResourceType eResourceType = GetResourceType<T>();

	if (eResourceType == E_ResourceType::END) {
		assert(nullptr && _T("Resource의 타입을 지정하지 않음"));
		return nullptr;
	}

	auto iter = m_umapResource[(UINT)eResourceType].find(_strKey);
	auto endIter = m_umapResource[(UINT)eResourceType].end();

	if (iter == endIter) {
		assert(nullptr && _T("Resource를 찾지 못함."));
		return nullptr;
	}
	T* pResource = dynamic_cast<T*>(iter->second);

	if (nullptr == pResource)
		assert(nullptr && _T("Resource를 찾지 못함."));

	return pResource;
}